// ArbiterPlus 独立服务器工程 —— c-s/common/inetwork.h
#ifndef AP_INETWORK_H
#define AP_INETWORK_H

#include <string>
#include <list>
#include <map>
#include <set>
#include <semaphore.h>
#include <stdint.h>

class INetwork;

struct PacketHeader {
    uint16_t m_seq;
    uint16_t m_len;
    uint8_t  m_code;
    uint8_t  m_status;
};

enum PacketCode {
    CODE_REQUEST   = 0x01,
    CODE_MORE      = 0x02,
    CODE_NEEDRESP  = 0x04,
    CODE_QUIET     = 0x08,
    CODE_RESP_DATA = 0x10,
    CODE_RESP_RES  = 0x20,
    CODE_RECEIPT   = 0x40,
};

class Config;
struct NetworkConfig {
    uint16_t m_serverPort;
    uint16_t m_clientPort;
    uint16_t m_localFilePort;
    uint16_t m_remoteFilePort;
    uint16_t m_selfevalPort;
    NetworkConfig();
};

class PacketHandler {
public:
    virtual ~PacketHandler() {}
    virtual void handle(INetwork* network, const char* from, PacketHeader* header,
                        char* data, unsigned from_port) = 0;
};

class INetwork {
public:
    INetwork(int local_port, int remote_port);
    ~INetwork();

    bool initSocket();
    void run();
    void runUDPServer();
    bool recvPacket();
    void handlePacket(const char* from, PacketHeader* header, char* data,
                      unsigned from_port);

    int  sendPacket(const std::string& target, int seq, char* command,
                    uint8_t code, uint8_t status, unsigned port = 0);

    int  waitResponse(int seq, std::string& from, PacketHeader& header,
                      std::string& data, unsigned timeout);

    void installHandler(PacketHandler* h);
    void setVerbose(bool v) { m_verbose = v; }
    void setBcast(const std::string& b) { m_bcast = b; }
    unsigned allocSeq() { return m_seq++; }
    void dumpPacket(PacketHeader* header, char* data);

private:
    struct Packet {
        std::string   m_src;
        PacketHeader  m_header;
        std::string   m_data;
    };
    struct WaitData {
        sem_t               m_sem;
        std::list<Packet>   m_packets;
    };
    WaitData* addWait(int seq);
    WaitData* queryWait(int seq);
    void      removeWait(int seq);

    std::list<PacketHandler*>          m_handlerList;
    std::map<uint16_t, WaitData*>      m_waitMap;
    uint16_t  m_seq;
    int       m_sockfd;
    sem_t     m_mutex;
    sem_t     m_hlock;
    uint16_t  m_udpLocalPort;
    uint16_t  m_udpRemotePort;
    bool      m_verbose;
    std::string m_bcast;
};

#endif
