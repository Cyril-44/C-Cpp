// ArbiterPlus 独立服务器工程 —— c-s/common/inetwork.cpp
#include "inetwork.h"
#include "config.h"
#include "log.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <cstdio>
#include <ctime>
#include <time.h>
#include <errno.h>
#include <unistd.h>

static inline void sem_wait_eintr(sem_t* s)
{ while (sem_wait(s) == -1 && errno == EINTR) {} }

NetworkConfig::NetworkConfig()
{
    Config conf("/etc/arbiter.conf");
    m_serverPort     = (uint16_t)conf.getNumericValue("network ServerPort",     7729);
    m_clientPort     = (uint16_t)conf.getNumericValue("network ClientPort",     7730);
    m_localFilePort  = (uint16_t)conf.getNumericValue("network LocalFilePort",  7731);
    m_remoteFilePort = (uint16_t)conf.getNumericValue("network RemoteFilePort", 7731);
    m_selfevalPort   = (uint16_t)conf.getNumericValue("network SelfEvalPort",   7732);
}

INetwork::INetwork(int local_port, int remote_port)
    : m_seq(0), m_sockfd(-1), m_udpLocalPort((uint16_t)local_port),
      m_udpRemotePort((uint16_t)remote_port), m_verbose(false),
      m_bcast("255.255.255.255")
{
    sem_init(&m_mutex, 0, 1);
    sem_init(&m_hlock, 0, 1);
}

INetwork::~INetwork()
{
    if (m_sockfd != -1) close(m_sockfd);
    sem_destroy(&m_mutex);
    sem_destroy(&m_hlock);
}

bool INetwork::initSocket()
{
    m_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (m_sockfd == -1) {
        APLog::printf(System, "failed to create socket for UDP");
        perror("socket");
        return false;
    }
    int on = 1;
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) == -1) {
        APLog::printf(System, "failed to set broadcast mode");
        perror("setsockopt");
    }
    sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family      = AF_INET;
    address.sin_port        = htons(m_udpLocalPort);
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(m_sockfd, (sockaddr*)&address, sizeof(address)) == -1) {
        APLog::printf(Alarm, "failed to bind UDP socket");
        perror("bind");
        return false;
    }
    return true;
}

void INetwork::run() { runUDPServer(); }

void INetwork::runUDPServer()
{
    do {
        APLog::printf(Debug, "waiting packet...");
    } while (recvPacket());
}

bool INetwork::recvPacket()
{
    char buffer[256];
    sockaddr_in address;
    socklen_t addr_len = sizeof(address);
    memset(&address, 0, sizeof(address));

    int len;
    do {
        len = (int)recvfrom(m_sockfd, buffer, 0xff, 0, (sockaddr*)&address, &addr_len);
    } while (len < 1 && errno == EINTR);

    if (len < 1) {
        APLog::printf(System, "failed to read packet");
        return false;
    }

    PacketHeader* header = (PacketHeader*)buffer;
    buffer[len] = '\0';
    if (m_verbose)
        dumpPacket(header, buffer + 6);

    if (len == (int)header->m_len) {
        APLog::printf(Debug, "recv packet from %s", inet_ntoa(address.sin_addr));
        handlePacket(inet_ntoa(address.sin_addr), header, buffer + 6,
                     ntohs(address.sin_port));
    } else {
        APLog::printf(Alarm, "broken packet from %s", inet_ntoa(address.sin_addr));
    }
    return true;
}

void INetwork::handlePacket(const char* from, PacketHeader* header, char* data,
                            unsigned from_port)
{
    bool forwarded = false;
    if ((header->m_code & CODE_REQUEST) == 0) {
        sem_wait_eintr(&m_mutex);
        WaitData* w = queryWait(header->m_seq);
        if (w != NULL) {
            APLog::printf(Debug, "forward response");
            Packet p;
            p.m_src    = from;
            p.m_header = *header;
            p.m_data   = data;
            w->m_packets.push_back(p);
            sem_post(&w->m_sem);
            forwarded = true;
        }
        sem_post(&m_mutex);
    }
    if (!forwarded) {
        sem_wait_eintr(&m_hlock);
        for (std::list<PacketHandler*>::iterator it = m_handlerList.begin();
             it != m_handlerList.end(); ++it)
            (*it)->handle(this, from, header, data, from_port);
        sem_post(&m_hlock);
    }
}

int INetwork::sendPacket(const std::string& target, int seq, char* command,
                         uint8_t code, uint8_t status, unsigned port)
{
    size_t cmdlen = strlen(command);
    unsigned packetlen = (unsigned)cmdlen + 6;
    if (packetlen >= 0x100) {
        APLog::printf(Alarm, "failed to send packet: max length exceeded (%d)", packetlen);
        return -1;
    }

    sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port   = htons(port == 0 ? m_udpRemotePort : port);
    address.sin_addr.s_addr =
        inet_addr(target.empty() ? m_bcast.c_str() : target.c_str());

    bool wait = (code & CODE_REQUEST) && (code & CODE_NEEDRESP);

    char* buf = new char[packetlen];
    uint16_t useq = (seq == -1) ? m_seq : (uint16_t)seq;
    memcpy(buf + 0, &useq, 2);                    // m_seq (LE)
    uint16_t plen = (uint16_t)packetlen;
    memcpy(buf + 2, &plen, 2);                    // m_len
    buf[4] = (char)code;
    buf[5] = (char)status;
    memcpy(buf + 6, command, cmdlen);

    if (wait)
        addWait(useq);

    bool ok = false;
    if (sendto(m_sockfd, buf, packetlen, 0, (const sockaddr*)&address,
               sizeof(address)) == -1) {
        APLog::printf(Alarm, "failed to send command (\"%s\") due to %s", command,
                      strerror(errno));
        perror("sendto");
    } else {
        APLog::printf(Normal, "send command (\"%s\") to %s", command,
                      target.empty() ? "broadcast" : target.c_str());
        ok = true;
    }

    int result;
    if (ok) {
        if (seq == -1)
            m_seq = m_seq + 1;
        result = useq;
    } else {
        if (wait)
            removeWait(useq);
        result = -1;
    }
    delete[] buf;
    return result;
}

int INetwork::waitResponse(int seq, std::string& from, PacketHeader& header,
                           std::string& data, unsigned timeout)
{
    sem_wait_eintr(&m_mutex);
    WaitData* w = queryWait(seq);
    sem_post(&m_mutex);

    if (w == NULL) {
        APLog::printf(Alarm, "try to wait for non-synchronized request");
        return -1;
    }

    timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
        APLog::printf(Alarm, "failed to get realtime via clock_gettime");
        perror("clock_gettime");
        return -1;
    }
    ts.tv_sec += timeout;

    int res = -1;
    if (sem_timedwait(&w->m_sem, &ts) == 0) {
        sem_wait_eintr(&m_mutex);
        if (!w->m_packets.empty()) {
            Packet& p = w->m_packets.front();
            from   = p.m_src;
            header = p.m_header;
            data   = p.m_data;
            w->m_packets.pop_front();
            res = 0;
        }
        sem_post(&m_mutex);
    }
    if (res == -1)
        APLog::printf(Debug, "failed (or timeout) to wait response");
    return res;
}

void INetwork::installHandler(PacketHandler* h)
{
    sem_wait_eintr(&m_hlock);
    m_handlerList.push_back(h);
    sem_post(&m_hlock);
}

INetwork::WaitData* INetwork::addWait(int seq)
{
    sem_wait_eintr(&m_mutex);
    WaitData* w = queryWait(seq);
    if (w == NULL) {
        w = new WaitData();
        sem_init(&w->m_sem, 0, 0);
        m_waitMap[(uint16_t)seq] = w;
    }
    sem_post(&m_mutex);
    return w;
}

INetwork::WaitData* INetwork::queryWait(int seq)
{
    std::map<uint16_t, WaitData*>::iterator it = m_waitMap.find((uint16_t)seq);
    return it == m_waitMap.end() ? NULL : it->second;
}

void INetwork::removeWait(int seq)
{
    sem_wait_eintr(&m_mutex);
    std::map<uint16_t, WaitData*>::iterator it = m_waitMap.find((uint16_t)seq);
    if (it != m_waitMap.end()) {
        WaitData* w = it->second;
        m_waitMap.erase(it);
        sem_destroy(&w->m_sem);
        delete w;
    }
    sem_post(&m_mutex);
}

void INetwork::dumpPacket(PacketHeader* header, char* data)
{
    APLog::printf(Debug, "packet: seq=%d len=%d code=0x%x status=%d data=%s",
                  header->m_seq, header->m_len, header->m_code, header->m_status, data);
}
