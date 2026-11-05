// ArbiterPlus 独立服务器工程 —— c-s/common/commoncmd.cpp
#include "commoncmd.h"
#include "log.h"
#include <cstdlib>

CommandRes VersionCmd::run(CommandContext* cc, Notifier*)
{
    cc->printf("%s\n", m_version.c_str());
    return CommandRes(0, "");
}

CommandRes HelpCmd::run(CommandContext* cc, Notifier*)
{
    const std::vector<Command*>& cmds = m_manager->commands();
    for (size_t i = 0; i < cmds.size(); i++) {
        Command* c = cmds[i];
        cc->printf("%-40s", c->command().empty() ? c->token().c_str()
                                                 : c->command().c_str());
        if (!c->command().empty())
            cc->printf("\n");
    }
    return CommandRes(0, "");
}

// file [-s] [-m mode] [-u owner] [-v] local remote dest
CommandRes FileCmd::run(CommandContext* cc, Notifier* nf)
{
    if (m_params.paramCount() != 3) {
        cc->printf("\t^unrecognized format\n");
        return CommandRes(-2000, "");
    }

    bool send = m_params.option('s');
    FDescriptor fd;
    fd.m_localPath  = m_params.param(0);
    fd.m_remotePath = m_params.param(1);
    fd.m_owner      = m_params.option('u').value();
    std::string modestr = m_params.option('m').value();
    if (!modestr.empty())
        fd.m_mode = (int)strtol(modestr.c_str(), NULL, 8);

    std::string dest = m_params.param(2);
    std::string ip   = dest.substr(0, dest.find(':'));

    FClient client(m_fm);
    if (client.request(ip, send ? Put : Get, &fd, nf))
        return CommandRes(0, "");

    char buf[256];
    snprintf(buf, sizeof(buf), "failed to transfer %s", fd.m_localPath.c_str());
    return CommandRes(-1002, buf);
}
