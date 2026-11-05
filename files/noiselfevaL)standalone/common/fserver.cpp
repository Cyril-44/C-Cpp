// ArbiterPlus 独立服务器工程 —— c-s/common/fserver.cpp
#include "fserver.h"
#include "arch.h"
#include "log.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <sstream>
#include <fstream>

// ================= 线协议 =================

Type FileManager::readCommand(int sock, std::string& value)
{
    Type type = Unkown;
    int len = 0;
    char* buffer = NULL;

    if (read(sock, &len, 4) != 4 || len < 2 || (buffer = new char[len]) == NULL ||
        read(sock, buffer, len) != len) {
        delete[] buffer;
        perror("read");
        return Unkown;
    }

    APLog::printf(Debug, "read file command: \"%s\"", buffer);
    if (buffer[len - 1] == '\0') {
        char* p = strchr(buffer, ' ');
        if (p == NULL)
            p = buffer + len - 1;
        else
            *p = '\0';
        value = p + 1;

        if      (strcmp(buffer, "put")     == 0) type = Put;
        else if (strcmp(buffer, "get")     == 0) type = Get;
        else if (strcmp(buffer, "ok")      == 0) type = Ok;
        else if (strcmp(buffer, "deny")    == 0) type = Deny;
        else if (strcmp(buffer, "echo")    == 0) type = Echo;
        else if (strcmp(buffer, "forward") == 0) type = Forward;
    }
    delete[] buffer;
    return type;
}

bool FileManager::writeCommand(int sock, const std::string& cmd)
{
    int len = (int)cmd.size() + 1;
    if (write(sock, &len, 4) != 4)
        return false;
    if (write(sock, cmd.c_str(), len) != len)
        return false;
    return true;
}

Type FileManager::readRequest(int sock, FDescriptor* fd)
{
    std::string value;
    Type t = readCommand(sock, value);
    if (t == Put || t == Get) {
        std::istringstream input(value);
        input >> fd->m_id >> fd->m_size >> fd->m_flag
              >> fd->m_localPath >> fd->m_remotePath >> fd->m_owner;
        input >> std::oct >> fd->m_mode >> std::dec;
        if (!input.eof())
            input >> fd->m_filter;
    }
    return t;
}

bool FileManager::writeRequest(int sock, Type type, FDescriptor* fd)
{
    if (type != Put && type != Get)
        return false;
    std::ostringstream ss;
    ss << (type == Put ? "put " : "get ")
       << fd->m_id << " " << fd->m_size << " " << fd->m_flag << " "
       << fd->m_localPath << " " << fd->m_remotePath << " " << fd->m_owner << " "
       << std::oct << fd->m_mode << std::dec;
    if (!fd->m_filter.empty())
        ss << " " << fd->m_filter;

    std::string cmd = ss.str();
    if (!writeCommand(sock, cmd)) {
        APLog::printf(Alarm, "failed to write command \"%s\"", cmd.c_str());
        return false;
    }
    return true;
}

unsigned FileManager::copyFile(const std::string& server, int srcfd, int destfd,
                               unsigned size, const std::string& src,
                               const std::string& dest, Notifier* nf)
{
    char* buf = new char[GlobalConfig::m_chunk];
    unsigned prevc = 0, c = 0;

    notifyProgress(server, src, dest, 0, nf);
    while (c < size) {
        int r;
        do {
            r = (int)read(srcfd, buf, GlobalConfig::m_chunk);
        } while (r < 1 && errno == EINTR);

        if (r <= 0)
            break;
        if (nf && nf->canceled())
            break;

        int wn = 0;
        while (wn < r) {
            int w = (int)write(destfd, buf + wn, r - wn);
            if (w == -1) break;
            wn += w;
        }
        c += r;
        unsigned pc = size ? (c * 100 / size) : 100;
        if (pc != prevc)
            notifyProgress(server, src, dest, pc, nf);
        prevc = pc;
    }
    delete[] buf;
    return c;
}

void FileManager::notifyProgress(const std::string& server, const std::string& src,
                                 const std::string& dest, unsigned percent, Notifier* nf)
{
    if (nf) {
        char buf[64];
        snprintf(buf, sizeof(buf), "%d%%", percent);
        nf->notify(Progress, src + "->" + dest + " " + buf);
    }
}

// ================= FServer =================

void FServer::run()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        APLog::printf(Alarm, "failed to create socket for file server");
        return;
    }
    int on = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family      = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port        = htons(m_port);
    if (bind(sockfd, (sockaddr*)&address, sizeof(address)) == -1) {
        perror("bind");
        APLog::printf(Alarm, "failed to bind for file server");
        close(sockfd);
        return;
    }
    if (listen(sockfd, 5) == -1) {
        perror("listen");
        APLog::printf(Alarm, "failed to listen for file server");
        close(sockfd);
        return;
    }
    APLog::printf(Normal, "file server listening on port %d", m_port);

    for (;;) {
        sockaddr_in client_address;
        socklen_t len = sizeof(client_address);
        int clientSock = accept(sockfd, (sockaddr*)&client_address, &len);
        if (clientSock == -1)
            continue;

        std::string src = inet_ntoa(client_address.sin_addr);
        APLog::printf(Debug, "client %s connects to file server", src.c_str());

        char name[64];
        snprintf(name, sizeof(name), "client(%s) handler", src.c_str());

        if (!m_multi) {
            FClientHandler handler(m_manager, src, clientSock);
            handler.run();
        } else {
            FClientHandler* h = new FClientHandler(m_manager, src, clientSock);
            Thread<FClientHandler>* t = new Thread<FClientHandler>(h, name);
            t->startAway();
        }
    }
}

// ================= FClientHandler =================
void FClientHandler::run()
{
    FDescriptor fd;
    int err = 0;
    int localfd = -1;
    std::string local;
    char buf[64];

    Type type = FileManager::readRequest(m_sock, &fd);
    if (type == Get || type == Put) {
        err = m_manager->confirmFileRequest(m_src, &fd);
        if (err == 0) {
            HostEnv::expandPath(fd.m_remotePath);
            local = fd.m_remotePath;
            if (type == Get && (err = m_manager->initRequest(&fd, local)) != 0)
                APLog::printf(Debug, "initRequest failed");
            if (type == Put && (fd.m_flag & 1)) {
                local = DirCache::inst()->createDirFileName(local);
            }
        }

        if (err == 0 && type == Put) {
            APLog::printf(Debug, "creat file %s", local.c_str());
            localfd = creat(local.c_str(), fd.m_mode);
        } else if (err == 0) {
            localfd = open(local.c_str(), O_RDONLY);
        }
        if (localfd == -1 && err == 0) {
            APLog::printf(System, "failed to open %s", local.c_str());
            err = -1000;
        }

        if (err == 0)
            snprintf(buf, sizeof(buf), "ok %ld %d", fd.m_size, fd.m_flag & 1);
        else
            snprintf(buf, sizeof(buf), "deny %d", err);

        if (!FileManager::writeCommand(m_sock, buf)) {
            APLog::printf(Debug, "failed to send confirm command to %s", m_src.c_str());
            err = -101;
        }

        if (err == 0) {
            unsigned c;
            if (type == Put)
                c = FileManager::copyFile(m_src, m_sock, localfd, (unsigned)fd.m_size,
                                          local, fd.m_localPath, NULL);
            else
                c = FileManager::copyFile(m_src, localfd, m_sock, (unsigned)fd.m_size,
                                          local, fd.m_localPath, NULL);
            close(localfd);
            if (c != (unsigned)fd.m_size)
                err = -1002;

            if (err == 0 && type == Put &&
                (err = m_manager->completeRequest(&fd, local, fd.m_remotePath)) != 0)
                APLog::printf(Debug, "completeRequest failed");

            if (type == Put) {
                snprintf(buf, sizeof(buf), "echo %d", err);
                FileManager::writeCommand(m_sock, buf);
            }
        }

        APLog::printf(Debug, "transfering file [%s%s%s:%s] seems %s",
                      local.c_str(), type == Put ? "->" : "<-", m_src.c_str(),
                      fd.m_localPath.c_str(), err == 0 ? "fine" : "broken");
    } else {
        APLog::printf(Alarm, "failed to read request from client %s", m_src.c_str());
    }
    close(m_sock);
}

// ================= FClient =================
bool FClient::request(const std::string& server, Type type, FDescriptor* fd,
                      Notifier* nf)
{
    int sockfd = -1, localfd = -1;
    std::string response, local;
    int err = 0;

    if (server.empty())
        return false;

    HostEnv::expandPath(fd->m_localPath);
    local = fd->m_localPath;
    if (type == Put && (err = m_manager->initRequest(fd, local)) != 0) {
        APLog::printf(Debug, "initRequest failed");
        return false;
    }
    if (nf && nf->canceled()) {
        err = -7;
        return false;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        APLog::printf(System, "failed to create socket for file client");
        return false;
    }

    sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port   = htons(m_manager->remotePort());
    address.sin_addr.s_addr = inet_addr(server.c_str());

    do {
        if (nf && nf->canceled()) { err = -7; break; }
        if (connect(sockfd, (sockaddr*)&address, sizeof(address)) == -1) {
            perror("connect");
            APLog::printf(Alarm, "failed to connect to remote file server \"%s\"",
                          server.c_str());
            err = -3;
            break;
        }
        if (!FileManager::writeRequest(sockfd, type, fd)) {
            err = -4;
            break;
        }

        Type t = FileManager::readCommand(sockfd, response);
        long size = 0;
        int flag = 0;
        if (t == Ok && sscanf(response.c_str(), "ok %ld %d", &size, &flag) == 2) {
            if (type == Put) {
                localfd = open(local.c_str(), O_RDONLY);
            } else {
                localfd = creat(local.c_str(), fd->m_mode ? fd->m_mode : 0644);
            }
            if (localfd == -1) {
                APLog::printf(System, "failed to open %s", local.c_str());
                err = -1000;
                break;
            }
            unsigned c;
            if (type == Put)
                c = FileManager::copyFile(server, localfd, sockfd, (unsigned)size,
                                          local, fd->m_remotePath, nf);
            else
                c = FileManager::copyFile(server, sockfd, localfd, (unsigned)size,
                                          fd->m_remotePath, local, nf);
            close(localfd);
            if (c != (unsigned)size)
                err = -1002;

            if (type == Put) {                    // 等待 echo 回执
                std::string fin;
                if (FileManager::readCommand(sockfd, fin) == Echo)
                    sscanf(fin.c_str(), "echo %d", &err);
                else
                    err = -5;
            }
        } else if (t == Deny) {
            sscanf(response.c_str(), "deny %d", &err);
        } else {
            err = -5;
        }
    } while (false);

    close(sockfd);
    if (err == 0 && type == Put) {
        m_manager->completeRequest(fd, local, fd->m_remotePath);
        m_manager->getSent().push_back(FileManager::FDElem());   // 记账(简化)
        m_manager->getSent().back().m_time = time(NULL);
        m_manager->getSent().back().m_src  = server;
        m_manager->getSent().back().m_fd   = *fd;
    }
    return err == 0;
}

// ================= FileManager =================

FileManager::FileManager()
    : m_localPort(0), m_remotePort(0), m_multi(false), m_taskId(0) {}

FileManager::~FileManager() {}

void FileManager::init(int local_port, int remote_port, bool multi)
{
    m_localPort = local_port;
    m_remotePort = remote_port;
    m_multi = multi;
}

int FileManager::confirmFileRequest(const std::string& src, FDescriptor* fd)
{
    (void)src; (void)fd;
    return 0;
}

int FileManager::initRequest(FDescriptor* fd, const std::string& local)
{
    (void)fd;
    struct stat st;
    if (stat(local.c_str(), &st) == 0) {
        fd->m_size = st.st_size;
        return 0;
    }
    return -1000;
}

int FileManager::completeRequest(FDescriptor* fd, const std::string& local,
                                 const std::string& remote)
{
    if (!fd->m_owner.empty() || fd->m_mode)
        setFileProperty(local, fd->m_owner, fd->m_mode);
    FDElem e;
    e.m_time = time(NULL);
    e.m_src  = remote;
    e.m_fd   = *fd;
    m_recved.push_back(e);
    notifyFileRecv(fd);
    return 0;
}

void FileManager::notifyFileRecv(FDescriptor* fd)
{
    for (size_t i = 0; i < m_notifiers.size(); i++)
        m_notifiers[i]->notify(0, fd->m_remotePath, fd->m_size);
}

void FileManager::reset() { m_recved.clear(); m_sent.clear(); m_taskId = 0; }

std::string FileManager::calcCheckSum(const std::string& file)
{
    std::string cmd = "md5sum " + file + " 2>/dev/null";
    FILE* p = popen(cmd.c_str(), "r");
    std::string sum;
    if (p) {
        char buf[64] = {0};
        if (fread(buf, 1, 32, p) == 32)
            sum = std::string(buf, 32);
        pclose(p);
    }
    return sum;
}

void FileManager::addCheckSum(const std::string& path, const std::string& sum)
{ m_checkSums.push_back(std::make_pair(path, sum)); }

bool FileManager::queryCheckSum(const std::string& path, const std::string& sum)
{
    for (size_t i = 0; i < m_checkSums.size(); i++)
        if (m_checkSums[i].first == path)
            return m_checkSums[i].second == sum;
    return false;
}

void FileManager::loadCheckSums(const std::string& file)
{
    std::ifstream in(file.c_str());
    std::string path, sum;
    while (in >> sum >> path)
        addCheckSum(path, sum);
}

void FileManager::saveCheckSums(const std::string& file)
{
    std::ofstream out(file.c_str());
    for (size_t i = 0; i < m_checkSums.size(); i++)
        out << m_checkSums[i].second << "  " << m_checkSums[i].first << "\n";
}

void FileManager::setFileProperty(const std::string& path, const std::string& owner,
                                  int mode)
{
    if (!owner.empty()) {
        std::string cmd = "chown " + owner + " '" + path + "'";
        system(cmd.c_str());
    }
    if (mode) {
        char mbuf[16];
        snprintf(mbuf, sizeof(mbuf), "%o", mode);
        std::string cmd = "chmod " + std::string(mbuf) + " '" + path + "'";
        system(cmd.c_str());
    }
}

long FileManager::fileLength(const std::string& path)
{
    struct stat st;
    return stat(path.c_str(), &st) == 0 ? st.st_size : -1;
}

// ================= DirCache =================

DirCache* DirCache::inst()
{
    static DirCache inst_;
    return &inst_;
}

std::string DirCache::createDirFileName(const std::string& path) { return path; }
std::string DirCache::dirToFile(const std::string& dir)  { return dir; }
std::string DirCache::fileToDir(const std::string& file) { return file; }
time_t DirCache::mtime(const std::string& path)
{
    struct stat st;
    return stat(path.c_str(), &st) == 0 ? st.st_mtime : 0;
}
void DirCache::updateDirFile(const std::string&, const std::string&) {}
