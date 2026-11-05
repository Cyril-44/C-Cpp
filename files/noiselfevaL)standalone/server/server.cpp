// ArbiterPlus 独立服务器工程 —— c-s/server/server.cpp
#include "server.h"
#include "log.h"
#include <unistd.h>
#include <sys/stat.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <set>

static const char* kServerVersion =
    "ArbiterPlus Standalone Server (rebuilt from binaries) " __DATE__ " " __TIME__;

Server::~Server()
{
    // 队列/线程随进程结束; 不做复杂清理
}

bool Server::init(const char* file, SelfEvalNotifier* selfeval_nf)
{
    APLog::init(file);

    loadConfig("./server.config");

    m_cm.registerCommand(new VersionCmd(kServerVersion));
    m_cm.registerCommand(new HelpCmd(&m_cm));
    m_cm.registerCommand(new SendCmd());
    m_cm.registerCommand(new FileCmd(&m_fm));
    m_cm.registerCommand(new MUploadCmd(this));
    m_cm.registerCommand(new SelfEvalResultCmd(selfeval_nf));

    m_network = new INetwork(m_networkConfig.m_serverPort, m_networkConfig.m_clientPort);
    if (!m_network->initSocket()) {
        APLog::printf(System, "failed to init network (port %d in use?)",
                      m_networkConfig.m_serverPort);
        exit(-1);
    }
    m_network->setVerbose(true);

    CommandPacketHandler* handler = new CommandPacketHandler(&m_cm, &m_queue);
    m_network->installHandler(handler);
    m_network->setBcast(m_bcast);

    m_fm.init(m_networkConfig.m_localFilePort, m_networkConfig.m_remoteFilePort, true);
    m_fserver = new FServer(&m_fm, m_networkConfig.m_localFilePort, true);

    if (m_localIP.empty())
        m_localIP = HostEnv::getLocalIP();

    m_networkThread = new Thread<INetwork>(m_network, "network");
    m_messageThread = new Thread<MessageQueue>(&m_queue, "message");
    m_fileThread    = new Thread<FServer>(m_fserver, "file server");
    m_networkThread->start();
    m_messageThread->start();
    m_fileThread->startAway();
    return true;
}

bool Server::localServerInit(const char* file)
{
    APLog::init(file);

    m_workDir  = "";
    m_localIP  = "";
    m_asyncMax = 10;
    m_bcast    = "255.255.255.255";

    m_cm.registerCommand(new VersionCmd(kServerVersion));
    m_cm.registerCommand(new HelpCmd(&m_cm));
    m_cm.registerCommand(new SendCmd());
    // 本地模式: 无 FileManager / mupload / selfevalresult

    m_network = new INetwork(m_networkConfig.m_serverPort, m_networkConfig.m_selfevalPort);
    if (!m_network->initSocket())
        return false;
    m_network->setVerbose(true);

    CommandPacketHandler* handler = new CommandPacketHandler(&m_cm, &m_queue);
    m_network->installHandler(handler);
    m_network->setBcast(m_bcast);

    if (m_localIP.empty())
        m_localIP = HostEnv::getLocalIP();

    m_networkThread = new Thread<INetwork>(m_network, "network");
    m_messageThread = new Thread<MessageQueue>(&m_queue, "message");
    m_networkThread->start();
    m_messageThread->start();
    return true;
}

void Server::loadConfig(const std::string& file)
{
    struct stat st;
    if (stat(file.c_str(), &st) == -1) {
        APLog::printf(Alarm, "server config file NOT exist, use default");
        return;
    }
    Config conf(file);
    m_workDir  = conf.getValue("network workdir", "");
    m_localIP  = conf.getValue("network localip", "");
    m_asyncMax = conf.getNumericValue("message threadmax", 10);
    m_bcast    = conf.getValue("network bcast", "255.255.255.255");
    GlobalConfig::m_asyncMax = m_asyncMax;
}

std::string Server::convertClientList(ClientList* cl)
{
    std::ostringstream ss;
    for (ClientList::iterator it = cl->begin(); it != cl->end(); ++it)
        ss << " " << *it;
    return ss.str();
}

int Server::execute(ClientList* cl, const std::string& cmd, Notifier* nf)
{
    if (cl->empty())
        return -2002;

    std::string command = cmd + convertClientList(cl);
    Command* c = m_cm.parseCommand(command.c_str());
    if (c == NULL)
        return -1;

    if (nf)
        nf->setCanceled(false);
    CommandContext* cc = new CommandContext(stdout, m_network, "", -1, 0);
    CommandMessage* m  = new CommandMessage(c, cc, nf);
    m->setSync(true);                       // 本地执行需同步等待结果
    m_queue.push(m);

    int res = c->wait();
    c->delRef();
    return res;
}

int Server::executeRaw(const std::string& cmd, Notifier* nf)
{
    Command* c = m_cm.parseCommand(cmd.c_str());
    if (c == NULL)
        return -1;
    if (nf)
        nf->setCanceled(false);
    CommandContext* cc = new CommandContext(stdout, m_network, "", -1, 0);
    CommandMessage* m  = new CommandMessage(c, cc, nf);
    m->setSync(true);
    m_queue.push(m);
    int res = c->wait();
    c->delRef();
    return res;
}

void Server::cancel(ClientList* cl)
{
    (void)cl;
}

void Server::asyncExecute(ClientList* cl, const std::string& cmd, Notifier* nf)
{
    if (cl->empty())
        return;
    std::string command = cmd + convertClientList(cl);
    Command* c = m_cm.parseCommand(command.c_str());
    if (c == NULL)
        return;
    CommandContext* cc = new CommandContext(stdout, m_network, "", -1, 0);
    CommandMessage* m  = new CommandMessage(c, cc, nf);
    m->setSync(false);
    m_queue.push(m);
    c->delRef();
}

// ================= ServerCmd::makeCmd =================
std::string ServerCmd::makeCmd(const std::string& cmd, const std::string& param)
{
    std::list<std::string> slist;
    std::string::size_type pos = 0;
    while (true) {
        std::string::size_type pn = param.find(':', pos);
        if (pn == std::string::npos) break;
        slist.push_back(param.substr(pos, pn - pos));
        pos = pn + 1;
    }
    slist.push_back(param.substr(pos));

    slist.pop_front();                       // 首项是目标 IP

    int id = 1;
    char buf[8];
    std::string out = cmd;
    for (std::list<std::string>::iterator it = slist.begin(); it != slist.end(); ++it) {
        snprintf(buf, sizeof(buf), "%%%d", id);
        std::string::size_type p = out.find(buf);
        if (p != std::string::npos)
            out.replace(p, strlen(buf), *it);
        id++;
    }
    return out;
}

// ================= SendCmd =================
CommandRes SendCmd::run(CommandContext* cc, Notifier* nf)
{
    bool broadcast = m_params.option('b');
    bool sync      = m_params.option('s');
    bool verbose   = m_params.option('v');
    int  timeout   = -1;

    if (m_params.option('t')) {
        timeout = atoi(m_params.option('t').value().c_str());
        if (timeout < 1) {
            cc->printf("\t^timeout can not be negative\n");
            return CommandRes(-4, "");
        }
    }
    if (m_params.paramCount() < 2) {
        std::string name = command();
        cc->printf("\t^unrecognized format ^%s\n", name.c_str());
        return CommandRes(-4, "");
    }

    std::string cmd = m_params.param(0);
    INetwork* network = cc->network();
    if (network == NULL) {
        cc->printf("\t^no network\n");
        return CommandRes(-1, "");
    }

    uint8_t code = CODE_REQUEST;
    if (sync || verbose) code = CODE_REQUEST | CODE_NEEDRESP | CODE_RECEIPT;   // 0x45
    if (verbose)         code |= CODE_QUIET;

    typedef std::set<std::string> SSet;
    SSet sset;
    int seq = network->allocSeq();

    for (int i = 1; i < m_params.paramCount(); i++) {
        std::string data = m_params.param(i);
        sset.insert(data.substr(0, data.find(':')));
    }

    if (broadcast) {
        bool failed = false;
        if (nf == NULL || !nf->canceled()) {
            if (network->sendPacket("", seq, (char*)cmd.c_str(), code, 0) == -1)
                failed = true;
        } else
            failed = true;
        if (failed) {
            cc->printf("\t^failed to broadcast command \"%s\"\n", cmd.c_str());
            if (nf) {
                Notifier::NResult nr(-1, "failed to broadcast command");
                for (SSet::iterator it = sset.begin(); it != sset.end(); ++it)
                    nf->notify(Result, *it, &nr);
            }
            sset.clear();
        }
    } else {
        for (int i = 1; i < m_params.paramCount(); i++) {
            std::string dest = m_params.param(i);
            std::string full = makeCmd(cmd, dest);
            std::string ip   = dest.substr(0, dest.find(':'));
            bool failed = false;
            if (nf == NULL || !nf->canceled()) {
                if (network->sendPacket(ip, seq, (char*)full.c_str(), code, 0) == -1)
                    failed = true;
            } else
                failed = true;
            if (failed) {
                cc->printf("\t^failed to send command \"%s\" to %s\n",
                           full.c_str(), dest.c_str());
                if (nf) {
                    Notifier::NResult nr(-1, "failed to send command");
                    nf->notify(Result, dest, &nr);
                }
                sset.erase(ip);
            }
        }
    }

    int ec = 0;
    while (!sset.empty()) {
        std::string src, data;
        PacketHeader header;
        if (network->waitResponse(seq, src, header, data, 1) == -1) {
            ec++;
        } else {
            ec = 0;
            if ((header.m_code & 0x30) == CODE_RESP_DATA) {
                if (nf) nf->notify(Progress, src, (void*)data.c_str());
            } else if ((header.m_code & 0x30) == CODE_RESP_RES) {
                int result = -1;
                char info[256] = {0};
                if (sscanf(data.c_str(), "&%d %255[^\n]", &result, info) >= 1) {
                    if (nf) {
                        Notifier::NResult nr(result, info);
                        nf->notify(Result, src, &nr);
                    }
                    sset.erase(src);
                } else {
                    cc->printf("\t^bad response from %s: %s\n", src.c_str(), data.c_str());
                }
            } else {
                APLog::printf(Debug, "unexpected packet header (code=0x%x) from %s",
                              header.m_code, src.c_str());
            }
        }
        if ((timeout != -1 && timeout <= ec) || (nf && nf->canceled()))
            break;
    }
    return CommandRes(0, "");
}

// ================= MUploadCmd =================
CommandRes MUploadCmd::run(CommandContext* cc, Notifier* nf)
{
    if (m_params.paramCount() < 3) {
        cc->printf("\t^unrecognized format\n");
        return CommandRes(-4, "");
    }
    std::string mode   = m_params.option('m').value();
    std::string owner  = m_params.option('u').value();
    std::string local  = m_params.param(0);
    std::string remote = m_params.param(1);

    for (int i = 2; i < m_params.paramCount(); i++) {
        std::string client = m_params.param(i);
        std::string cmd = "file -s -m " + (mode.empty() ? "0755" : mode);
        if (!owner.empty())
            cmd += " -u " + owner;
        cmd += " " + local + " " + remote + " " + client;

        Command* c = m_manager->parseCommand(cmd.c_str());
        int res = (c == NULL) ? -1 : c->execLocal(cc, NULL);
        notifyResult(nf, client, res);
        if (c) c->delRef();
    }
    return CommandRes(0, "");
}

void MUploadCmd::notifyResult(Notifier* nf, const std::string& client, int res)
{
    if (nf) {
        Notifier::NResult nr(res, "");
        nf->notify(Result, client, &nr);
    }
}

// ================= SelfEvalResultCmd =================
CommandRes SelfEvalResultCmd::run(CommandContext* cc, Notifier*)
{
    std::string user      = m_params.option('u').value();
    std::string day       = m_params.option('d').value();
    std::string timestamp = m_params.option('t').value();

    if (user.empty() || day.empty() || timestamp.empty()) {
        APLog::printf(Debug, "selfevalresult: unrecognized format from %s", cc->src());
        return CommandRes(-4, "");
    }

    m_selfeval_nf->notify(day, user, timestamp);
    return CommandRes(0, "");
}
