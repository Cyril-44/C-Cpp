// ArbiterPlus 独立服务器工程 —— c-s/common/fserver.h
#ifndef AP_FSERVER_H
#define AP_FSERVER_H

#include <string>
#include <vector>
#include <ctime>
#include <netinet/in.h>

#include "message.h"

enum Type { Unkown = 0, Put, Get, Ok, Deny, Echo, Forward };

class FDescriptor {
public:
    int         m_id;
    long        m_size;
    int         m_flag;
    std::string m_localPath;
    std::string m_remotePath;
    std::string m_owner;
    int         m_mode;
    std::string m_filter;
    FDescriptor() : m_id(0), m_size(0), m_flag(0), m_mode(0644) {}
    bool operator==(const FDescriptor& o) const { return m_id == o.m_id; }
};

class FNotifier {
public:
    virtual ~FNotifier() {}
    virtual void notify(int event, const std::string& path, long size) = 0;
    virtual void delRef() {}
};

class FileManager {
public:
    struct FDElem {                       // 嵌套结构 (与原始 DWARF 一致)
        time_t      m_time;
        std::string m_src;
        FDescriptor m_fd;
    };

    FileManager();
    ~FileManager();

    void init(int local_port, int remote_port, bool multi);
    int  localPort() const { return m_localPort; }
    int  remotePort() const { return m_remotePort; }

    static Type readCommand(int sock, std::string& value);
    static bool writeCommand(int sock, const std::string& cmd);
    static Type readRequest(int sock, FDescriptor* fd);
    static bool writeRequest(int sock, Type type, FDescriptor* fd);

    static unsigned copyFile(const std::string& server, int srcfd, int destfd,
                             unsigned size, const std::string& src,
                             const std::string& dest, Notifier* nf);

    int  confirmFileRequest(const std::string& src, FDescriptor* fd);
    int  initRequest(FDescriptor* fd, const std::string& local);
    int  completeRequest(FDescriptor* fd, const std::string& local,
                         const std::string& remote);
    void notifyFileRecv(FDescriptor* fd);
    static void notifyProgress(const std::string& server, const std::string& src,
                               const std::string& dest, unsigned percent, Notifier* nf);

    void addNotifier(FNotifier* n) { m_notifiers.push_back(n); }
    void reset();
    unsigned taskId() const { return m_taskId; }
    std::vector<FDElem>& getRecved() { return m_recved; }
    std::vector<FDElem>& getSent()   { return m_sent; }

    std::string calcCheckSum(const std::string& file);
    void        addCheckSum(const std::string& path, const std::string& sum);
    bool        queryCheckSum(const std::string& path, const std::string& sum);
    void        loadCheckSums(const std::string& file);
    void        saveCheckSums(const std::string& file);
    static void setFileProperty(const std::string& path, const std::string& owner,
                                int mode);
    static long fileLength(const std::string& path);

private:
    int  m_localPort;
    int  m_remotePort;
    bool m_multi;
    unsigned             m_taskId;
    std::vector<FDElem>  m_recved;
    std::vector<FDElem>  m_sent;
    std::vector<FNotifier*> m_notifiers;
    std::vector<std::pair<std::string, std::string> > m_checkSums;
};

class FServer {
public:
    FServer(FileManager* manager, int port, bool multi)
        : m_manager(manager), m_port(port), m_multi(multi) {}
    void run();
private:
    FileManager* m_manager;
    int          m_port;
    bool         m_multi;
};

class FClientHandler {
public:
    FClientHandler(FileManager* manager, const std::string& src, int sock)
        : m_manager(manager), m_src(src), m_sock(sock) {}
    void run();
private:
    FileManager* m_manager;
    std::string  m_src;
    int          m_sock;
};

class FClient {
public:
    FClient(FileManager* manager) : m_manager(manager) {}
    bool request(const std::string& server, Type type, FDescriptor* fd, Notifier* nf);
private:
    FileManager* m_manager;
};

class DirCache {
public:
    static DirCache* inst();
    std::string createDirFileName(const std::string& path);
    std::string dirToFile(const std::string& dir);
    std::string fileToDir(const std::string& file);
    time_t      mtime(const std::string& path);
    void        updateDirFile(const std::string& dir, const std::string& file);
};

#endif
