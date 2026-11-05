// ArbiterPlus 独立服务器工程 —— c-s/common/commoncmd.h
#ifndef AP_COMMONCMD_H
#define AP_COMMONCMD_H

#include "command.h"
#include "fserver.h"

// version: 打印版本串
class VersionCmd : public Command {
public:
    VersionCmd(const std::string& version) : m_version(version) {}
    Command* clone() { return new VersionCmd(m_version); }
    void initParam(const StringList&) {}
    std::string token() const { return "version"; }
    CommandRes run(CommandContext* cc, Notifier*);
private:
    std::string m_version;
};

// help [-v] [command]
class HelpCmd : public Command {
public:
    HelpCmd(CommandManager* manager) : m_manager(manager)
    { m_params.addOption('v', false, "display verbose information"); }
    Command* clone() { return new HelpCmd(m_manager); }
    void initParam(const StringList&) {}
    std::string token() const { return "help"; }
    std::string command() const { return "help [-v] [command]"; }
    CommandRes run(CommandContext* cc, Notifier*);
private:
    CommandManager* m_manager;
};

// file [-s] [-m mode] [-u owner] [-v] local-file remote-file destination
class FileCmd : public Command {
public:
    FileCmd(FileManager* fm) : m_fm(fm)
    {
        m_params.addOption('s', false, "send file if set, otherwise recv file");
        m_params.addOption('m', true,  "file mode, in octal");
        m_params.addOption('u', true,  "file owner, in user name");
        m_params.addOption('v', false, "print progress information");
    }
    Command* clone() { return new FileCmd(m_fm); }
    void initParam(const StringList&) {}
    std::string token() const { return "file"; }
    std::string command() const
    { return "file [-s] [-m mode] [-u owner] [-v] local-file remote-file destination"; }
    bool sync() const { return (bool)m_params.option('s'); }
    CommandRes run(CommandContext* cc, Notifier* nf);
private:
    FileManager* m_fm;
};

#endif
