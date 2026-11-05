// ArbiterPlus 独立服务器工程 —— c-s/common/arch.cpp
#include "arch.h"
#include "log.h"
#include <wordexp.h>
#include <unistd.h>
#include <errno.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>

unsigned GlobalConfig::m_chunk    = 4096;
int      GlobalConfig::m_asyncMax = 10;

// ---- HostEnv ----
bool HostEnv::expandPath(std::string& path)
{
    if (path.empty())
        return false;

    wordexp_t we;
    if (wordexp(path.c_str(), &we, 0) != 0)
        return false;
    path.clear();
    for (unsigned i = 0; i < we.we_wordc; i++)
        path += we.we_wordv[i];
    wordfree(&we);

    if (path.empty())
        return false;
    if (path[0] != '/') {                          // 相对路径补 cwd
        char cwd[4096];
        if (getcwd(cwd, sizeof(cwd)) == NULL)
            return false;
        path = std::string(cwd) + "/" + path;
    }
    return true;
}

std::string HostEnv::getLocalIP()
{
    FILE* p = popen("ifconfig 2>/dev/null | grep \"inet addr\" | "
                    "sed -e 's/[ \t]*inet addr:\\([0-9\\.]\+\\).*$/\\1/g' | "
                    "awk 'NR==1 {print}'", "r");
    std::string ip;
    if (p) {
        char buf[64] = {0};
        if (fread(buf, 1, sizeof(buf) - 1, p) > 0) {
            char* nl = strchr(buf, '\n');
            if (nl) *nl = 0;
            char* sp = strchr(buf, ' ');
            if (sp) *sp = 0;
            ip = buf;
        }
        pclose(p);
    }
    return ip;
}

std::string HostEnv::getBcast()
{
    FILE* p = popen("ifconfig 2>/dev/null | grep \"Bcast\" | "
                    "sed -e 's/.*Bcast:\\([0-9\\.]\+\\).*$/\\1/g' | "
                    "awk 'NR==1 {print}'", "r");
    std::string bc;
    if (p) {
        char buf[64] = {0};
        if (fread(buf, 1, sizeof(buf) - 1, p) > 0) {
            char* nl = strchr(buf, '\n');
            if (nl) *nl = 0;
            char* sp = strchr(buf, ' ');
            if (sp) *sp = 0;
            bc = buf;
        }
        pclose(p);
    }
    return bc;
}

// ---- Thread<T> ----
// 显式实例化 (匹配原始使用点; FServer/FClientHandler 由使用处隐式实例化)
