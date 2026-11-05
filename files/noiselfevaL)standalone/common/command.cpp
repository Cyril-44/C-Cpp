// ArbiterPlus 独立服务器工程 —— c-s/common/command.cpp
#include "command.h"
#include "log.h"
#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include <cerrno>

int CommandParam::split(const char* s, StringList& list)
{
    if (s == NULL)
        return 0;

    bool quote = false, escape = false;
    std::string param;
    list.clear();

    for (const char* p = s; *p != '\0'; ++p) {
        if (escape) {
            escape = false;
            param += *p;
        } else if (quote || *p != ' ') {
            if (*p == '\\')
                escape = true;
            else if (*p == '\"')
                quote = !quote;
            else
                param += *p;
        } else {
            if (!param.empty())
                list.push_back(param);
            param.clear();
        }
    }
    if (!param.empty())
        list.push_back(param);
    return (int)list.size();
}

void CommandParam::addOption(char option, bool arg, const std::string& help)
{
    m_options[option] = Option(option, arg, help);
}

std::string CommandParam::popOption(std::vector<std::string>& params, char option,
                                    bool arg)
{
    std::string value;
    char flag[3] = { '-', option, 0 };
    for (size_t i = 0; i < params.size(); i++) {
        if (params[i] == flag) {
            params.erase(params.begin() + i);
            if (arg && i < params.size()) {
                value = params[i];
                params.erase(params.begin() + i);
            } else {
                value = " ";                // 无参选项以非空标记"出现"
            }
            break;
        }
    }
    return value;
}

void CommandParam::init(const StringList& list)
{
    m_params = list;
    for (std::map<char, Option>::iterator it = m_options.begin();
         it != m_options.end(); ++it)
        it->second.m_value = popOption(m_params, it->second.m_option, it->second.m_arg);
    truncParam(m_params);
}

void CommandParam::truncParam(std::vector<std::string>& params)
{
    std::vector<std::string>::iterator it = params.begin();
    while (it != params.end()) {
        if ((*it).size() == 1 && (*it)[0] == '-')
            it = params.erase(it);
        else
            ++it;
    }
}

// ================= Command =================

std::string Command::query(int what) const
{
    if (what == Name)
        return token();
    return "";
}

int Command::exec(CommandContext* cc, bool async, Notifier* nf)
{
    CommandRes res;

    APLog::printf(Debug, "exec %s", command().c_str());
    if (nf)
        nf->notify(Begin, command());

    if (async && !canSync()) {
        res = CommandRes(-6, command());
    } else if (nf && nf->canceled()) {
        res = CommandRes(-7, command());
    } else {
        res = run(cc, nf);
    }

    cc->flush();

    if (cc->needResponse() && cc->src() != NULL && cc->network() != NULL) {
        char buf[256];
        buf[0] = '\0';
        snprintf(buf, sizeof(buf), "&%d %s", res.m_res, res.m_info.c_str());
        int r = cc->network()->sendPacket(cc->srcString(), cc->seq(), buf,
                                          CODE_RESP_RES, 0, cc->src_port());
        if (r == -1)
            APLog::printf(Alarm, "failed to send standard response to %s\n", cc->src());
    }

    m_res = res;
    if (nf)
        nf->notify(End, command());
    notify();
    if (m_manager)
        m_manager->addHistory(command(), res);
    return res.m_res;
}

int Command::execLocal(CommandContext* cc, Notifier* nf)
{
    CommandRes res;
    if (nf)
        nf->notify(Begin, command());
    res = run(cc, nf);
    m_res = res;
    if (nf)
        nf->notify(End, command());
    notify();
    return res.m_res;
}

// ================= CommandManager =================

CommandManager::~CommandManager()
{
    for (size_t i = 0; i < m_cmds.size(); i++)
        m_cmds[i]->delRef();
}

bool CommandManager::registerCommand(Command* c)
{
    m_cmds.push_back(c);
    return true;
}

void CommandManager::replaceMacro(std::string& cmd) const
{
    for (std::list<std::pair<std::string, std::string> >::const_iterator
         it = m_macros.begin(); it != m_macros.end(); ++it) {
        std::string macro = "$" + it->first;
        std::string::size_type pos;
        while ((pos = cmd.find(macro)) != std::string::npos)
            cmd.replace(pos, macro.size(), it->second);
    }
}

Command* CommandManager::parseCommand(const char* s)
{
    std::list<std::string> cmds;
    std::string sc;
    bool quote = false, escape = false;

    for (const char* p = s; *p != '\0'; ++p) {
        if (!quote && !escape && *p == '&' && p[1] == '&') {
            if (!sc.empty()) {
                cmds.push_back(sc);
                APLog::printf(Debug, "read command \"%s\"", sc.c_str());
            }
            sc.clear();
            ++p;
        } else {
            sc += *p;
            if (escape)
                escape = false;
            else if (*p == '\\')
                escape = true;
            else if (*p == '\"')
                quote = !quote;
        }
    }
    if (!sc.empty()) {
        cmds.push_back(sc);
        APLog::printf(Debug, "read command \"%s\"", sc.c_str());
    }

    bool ok = true;
    Command* cmd = NULL;
    Command* last = NULL;

    for (std::list<std::string>::iterator cit = cmds.begin(); cit != cmds.end(); ++cit) {
        std::string cmdline = *cit;
        StringList list;

        replaceMacro(cmdline);
        if (CommandParam::split(cmdline.c_str(), list) == 0) {
            ok = false;
            break;
        }
        std::string tokenstr = list.front();
        list.erase(list.begin());

        Command* c = NULL;
        for (size_t i = 0; i < m_cmds.size(); i++) {
            if (m_cmds[i]->token() == tokenstr) {
                c = m_cmds[i]->clone();
                c->params().init(list);
                c->setCommand(cmdline);
                c->m_manager = this;
            }
        }
        if (c != NULL) {
            if (cmd == NULL) { cmd = c; last = c; }
            else            { last->m_next = c; last = c; }
        }
    }

    if (!ok && cmd != NULL)
        cmd->delRef();
    return ok ? cmd : NULL;
}

// ================= CommandContext =================

CommandContext::CommandContext(FILE* output, INetwork* network, const std::string& src,
                               int seq, unsigned src_port)
    : m_output(output), m_network(network), m_src(src), m_seq(seq),
      m_src_port(src_port), m_needResponse(false), m_pos(0)
{
    m_buf[0] = '\0';
}

void CommandContext::printf(const char* format, ...)
{
    if (m_output != NULL) {
        va_list ap;
        va_start(ap, format);
        if (vfprintf(m_output, format, ap) < 0)
            perror("vfprintf");
        va_end(ap);
        return;
    }
    if (m_network == NULL || m_src.empty())
        return;

    char* buf = new char[0x400];
    va_list ap;
    va_start(ap, format);
    if (vsnprintf(buf, 0x400, format, ap) < 0)
        perror("vsnprintf");
    else
        feed(buf);
    va_end(ap);
    delete[] buf;
}

void CommandContext::feed(char* buf)
{
    while (*buf != '\0') {
        while (*buf != '\0' && m_pos < 0xf8)
            m_buf[m_pos++] = *buf++;
        m_buf[m_pos] = '\0';
        if (m_pos == 0xf8)
            send(true);
    }
}

void CommandContext::send(bool more)
{
    uint8_t code = more ? (CODE_RESP_DATA | CODE_MORE) : CODE_RESP_DATA;
    if (m_network->sendPacket(m_src, m_seq, m_buf, code, 0, m_src_port) == -1)
        APLog::printf(Alarm, "failed to send response to \"%s\"", m_src.c_str());
    m_pos = 0;
    m_buf[0] = '\0';
}

void CommandContext::flush()
{
    if (m_output != NULL) {
        fflush(m_output);
        return;
    }
    if (m_network != NULL && !m_src.empty())
        send(false);
}

// ================= CommandPacketHandler =================

void CommandPacketHandler::handle(INetwork* network, const char* from,
                                  PacketHeader* header, char* data, unsigned from_port)
{
    if ((header->m_code & CODE_REQUEST) == 0) {
        APLog::printf(Normal, "unexpected response from %s, drop", from);
        network->dumpPacket(header, data);
        return;
    }

    Command* c = m_manager->parseCommand(data);
    if (c == NULL) {
        APLog::printf(Alarm, "failed to parse command (%s)", data);
        return;
    }

    FILE* output = (header->m_code & CODE_QUIET) ? NULL : stdout;

    APLog::printf(Normal, "post command (%s)", data);
    CommandContext* cc = new CommandContext(output, network, from,
                                            header->m_seq, from_port);
    if (header->m_code & CODE_RECEIPT)
        cc->setResponse(true);

    CommandMessage* m = new CommandMessage(c, cc, NULL);
    m_queue->push(m);
    c->delRef();
}

// ================= CommandMessage =================

void CommandMessage::handle()
{
    APLog::printf(Debug, "execute command \"%s\"", m_cmd->command().c_str());
    m_cmd->exec(m_cc, runType() == Async, m_nf);
    delete m_cc;
}

std::string CommandMessage::query(int what) const
{
    if (what == Name)
        return m_cmd->command();
    return "";
}
