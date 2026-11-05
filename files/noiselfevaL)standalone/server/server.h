// ArbiterPlus 独立服务器工程 —— c-s/server/server.h
#ifndef AP_SERVER_H
#define AP_SERVER_H

#include <string>
#include <list>
#include "command.h"
#include "commoncmd.h"
#include "fserver.h"
#include "arch.h"
#include "config.h"

class SelfEvalNotifier {
public:
    virtual ~SelfEvalNotifier() {}
    virtual void notify(const std::string& day, const std::string& user,
                        const std::string& timestamp) = 0;
};

typedef std::list<std::string> ClientList;

class Server {
public:
    Server() : m_workDir(""), m_localIP(""), m_asyncMax(10),
               m_bcast("255.255.255.255"),
               m_network(NULL), m_fserver(NULL),
               m_networkThread(NULL), m_messageThread(NULL), m_fileThread(NULL) {}
    ~Server();

    // 完整模式: version/help/send/file/mupload/selfevalresult + 文件服务(7731)
    bool init(const char* logFile, SelfEvalNotifier* selfeval_nf);
    // 本地自测模式: 仅 version/help/send, 7729 -> 7732, 无文件服务
    bool localServerInit(const char* logFile);

    int  execute(ClientList* cl, const std::string& cmd, Notifier* nf);
    int  executeRaw(const std::string& cmd, Notifier* nf);   // 不追加目标, 命令串自负
    void cancel(ClientList* cl);
    void asyncExecute(ClientList* cl, const std::string& cmd, Notifier* nf);

    INetwork* network() { return m_network; }
    const NetworkConfig& networkConfig() const { return m_networkConfig; }

private:
    std::string convertClientList(ClientList* cl);
    void loadConfig(const std::string& file);

    std::string  m_workDir;
    std::string  m_localIP;
    int          m_asyncMax;
    std::string  m_bcast;
    Config       m_config;
    NetworkConfig m_networkConfig;

    CommandManager m_cm;
    MessageQueue   m_queue;
    FileManager    m_fm;
    INetwork*      m_network;
    FServer*       m_fserver;
    Thread<INetwork>*     m_networkThread;
    Thread<MessageQueue>* m_messageThread;
    Thread<FServer>*      m_fileThread;
};

class ServerCmd : public Command {
public:
    static std::string makeCmd(const std::string& cmd, const std::string& param);
};

class SendCmd : public ServerCmd {
public:
    SendCmd()
    {
        m_params.addOption('b', false, "broadcast");
        m_params.addOption('s', false, "sync request");
        m_params.addOption('v', false, "verbose mode (grap response from remote client)");
        m_params.addOption('t', true,  "specify timeout interval");
    }
    Command* clone() { return new SendCmd(); }
    void initParam(const StringList&) {}
    std::string token() const { return "send"; }
    std::string command() const
    { return "send [-b] [-s] [-v] [-t timeout] \"command\" ip-address:param1:param2 ..."; }
    bool sync() const { return (bool)m_params.option('s'); }
    CommandRes run(CommandContext* cc, Notifier* nf);
};

class MUploadCmd : public ServerCmd {
public:
    MUploadCmd(Server* server) : m_server(server)
    {
        m_params.addOption('m', true, "file mode, in octal");
        m_params.addOption('u', true, "file owner, in user name");
    }
    Command* clone() { return new MUploadCmd(m_server); }
    void initParam(const StringList&) {}
    std::string token() const { return "mupload"; }
    std::string command() const
    { return "mupload [-m mode] [-u owner] local-path remote-path [client...]"; }
    bool sync() const { return true; }
    CommandRes run(CommandContext* cc, Notifier* nf);
    void notifyResult(Notifier* nf, const std::string& client, int res);
private:
    Server* m_server;
};

class SelfEvalResultCmd : public ServerCmd {
public:
    SelfEvalResultCmd(SelfEvalNotifier* nf) : m_selfeval_nf(nf)
    {
        m_params.addOption('u', true, "user");
        m_params.addOption('t', true, "result timestamp");
        m_params.addOption('d', true, "the day of competition");
    }
    Command* clone() { return new SelfEvalResultCmd(m_selfeval_nf); }
    void initParam(const StringList&) {}
    std::string token() const { return "selfevalresult"; }
    std::string command() const
    { return "selfevalresult -u user -t timestamp -d day"; }
    CommandRes run(CommandContext* cc, Notifier* nf);
private:
    SelfEvalNotifier* m_selfeval_nf;
};

#endif
