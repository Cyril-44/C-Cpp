// ArbiterPlus 独立服务器工程 —— c-s/common/command.h
#ifndef AP_COMMAND_H
#define AP_COMMAND_H

#include <string>
#include <vector>
#include <map>
#include <list>
#include <utility>
#include <semaphore.h>
#include <cstdio>

#include "inetwork.h"
#include "message.h"

class Command;
class CommandManager;
class CommandContext;

typedef std::vector<std::string> StringList;

struct Option {
    char        m_option;
    bool        m_arg;
    std::string m_value;
    std::string m_help;
    Option() : m_option(0), m_arg(false) {}
    Option(char o, bool arg, const std::string& help)
        : m_option(o), m_arg(arg), m_help(help) {}
    const std::string& value() const { return m_value; }
    const std::string& help()  const { return m_help; }
    operator bool() const { return !m_value.empty(); }
};

class CommandParam {
public:
    CommandParam() {}

    void init(const StringList& list);
    static int  split(const char* s, StringList& list);
    Option& option(char op) { return m_options[op]; }
    const Option& option(char op) const
    {
        static Option none;
        std::map<char, Option>::const_iterator it = m_options.find(op);
        return it == m_options.end() ? none : it->second;
    }
    void addOption(char option, bool arg, const std::string& help);
    const std::string& optionHelp(char op) { return m_options[op].m_help; }
    const std::string& param(int i) const { return m_params[i]; }
    int  paramCount() const { return (int)m_params.size(); }
    const std::map<char, Option>& options() const { return m_options; }
    static void truncParam(std::vector<std::string>& params);

private:
    static std::string popOption(std::vector<std::string>& params, char option,
                                 bool arg);
    std::map<char, Option> m_options;
    std::vector<std::string> m_params;
};

struct CommandRes {
    int         m_res;
    std::string m_info;
    CommandRes() : m_res(0) {}
    CommandRes(int res, const std::string& info) : m_res(res), m_info(info) {}
};

typedef std::list<std::pair<std::string, CommandRes> > CommandHList;

// 注: 原始代码 QueryType 枚举含 Progress(=3), 与 Role 枚举的 Progress 同名冲突,
//     重建环境中二者同处全局命名空间, 故此处省略 Progress 成员 (未被使用)
enum QueryType { Name = 0, Format = 1, Help = 2 };

class Command {
public:
    Command() : m_ref(1), m_next(NULL), m_manager(NULL)
        { sem_init(&m_finish, 0, 0); }
    virtual ~Command() { sem_destroy(&m_finish); }

    virtual Command* clone() = 0;
    virtual void initParam(const StringList& list) = 0;
    virtual CommandRes run(CommandContext* cc, Notifier* nf) = 0;

    virtual std::string token()  const = 0;
    virtual std::string command() const { return m_cmdline; }
    virtual std::string query(int what) const;
    virtual bool canSync() const { return true; }
    virtual bool sync()    const { return true; }

    int  exec(CommandContext* cc, bool async, Notifier* nf);
    int  execLocal(CommandContext* cc, Notifier* nf);
    int  wait() { while (sem_wait(&m_finish) == -1 && errno == EINTR) {}
                  return m_res.m_res; }
    void notify() { sem_post(&m_finish); }

    void setCommand(const std::string& cmd) { m_cmdline = cmd; }
    void addRef() { m_ref++; }
    void delRef() { if (--m_ref < 1) delete this; }
    const CommandRes& result() const { return m_res; }
    CommandParam& params() { return m_params; }

protected:
    CommandParam   m_params;
    CommandRes     m_res;
    int            m_ref;
    Command*       m_next;
    CommandManager* m_manager;
    std::string    m_cmdline;
    sem_t          m_finish;
    friend class CommandManager;
};

class CommandManager {
public:
    CommandManager() {}
    ~CommandManager();

    bool registerCommand(Command* c);
    Command* parseCommand(const char* s);
    void replaceMacro(std::string& cmd) const;
    void addHistory(const std::string& cmd, const CommandRes& res)
        { m_history.push_back(std::make_pair(cmd, res)); }
    const CommandHList& history() const { return m_history; }
    const std::vector<Command*>& commands() const { return m_cmds; }

private:
    std::vector<Command*> m_cmds;
    CommandHList          m_history;
    std::list<std::pair<std::string, std::string> > m_macros;
};

class CommandContext {
public:
    CommandContext(FILE* output, INetwork* network, const std::string& src,
                   int seq, unsigned src_port);
    ~CommandContext() { flush(); }

    void printf(const char* format, ...);
    void feed(char* buf);
    void send(bool more);
    void flush();

    void setResponse(bool need) { m_needResponse = need; }
    bool needResponse() const   { return m_needResponse; }
    int  seq() const            { return m_seq; }
    const char* src() const     { return m_src.c_str(); }
    const std::string& srcString() const { return m_src; }
    unsigned src_port() const   { return m_src_port; }
    INetwork* network() const   { return m_network; }

private:
    FILE*       m_output;
    INetwork*   m_network;
    std::string m_src;
    int         m_seq;
    unsigned    m_src_port;
    bool        m_needResponse;
    char        m_buf[249];
    int         m_pos;
};

class CommandPacketHandler : public PacketHandler {
public:
    CommandPacketHandler(CommandManager* manager, MessageQueue* queue)
        : m_manager(manager), m_queue(queue) {}
    void handle(INetwork* network, const char* from, PacketHeader* header,
                char* data, unsigned from_port);
private:
    CommandManager* m_manager;
    MessageQueue*   m_queue;
};

class CommandMessage : public Message {
public:
    CommandMessage(Command* cmd, CommandContext* cc, Notifier* nf)
        : m_cmd(cmd), m_cc(cc), m_nf(nf) { setSync(cmd->sync()); }
    void handle();
    std::string query(int what) const;
    void delRef() { delete this; }
private:
    Command*        m_cmd;
    CommandContext* m_cc;
    Notifier*       m_nf;
};

#endif
