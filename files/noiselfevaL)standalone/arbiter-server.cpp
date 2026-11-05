// =====================================================================
// arbiter-server —— ArbiterPlus 独立服务器入口 (重建代码, 无 Qt 依赖)
//
// 等价于原 selfeval GUI 内嵌 Server 的完整模式 + 控制台命令入口:
//   - 监听 UDP 7729 (ServerPort), 向 ClientPort 7730 发命令
//   - TCP 7731 文件服务 (file/mupload)
//   - 注册命令: version help send file mupload selfevalresult
//
// 用法:
//   ./arbiter-server                        # 交互 REPL, 默认目标 127.0.0.1
//   ./arbiter-server 192.168.1.10 192.168.1.11   # 指定默认目标客户端
//   ./arbiter-server -c 'send -s -t 5 "tasknames" 127.0.0.1'   # 单发模式
//   ./arbiter-server --local                # 本地自测模式(仅 send, 7729->7732)
//
// REPL 示例:
//   send -s -t 5 "tasknames" 127.0.0.1
//   send -s -t 60 "selfeval -u <用户> align" 127.0.0.1
//   send -s "status" 192.168.1.10
//   mupload -m 0755 /home/evaluate/data.zip /home/evaluate/data.zip 127.0.0.1
//   help -v
// =====================================================================
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <list>

#include "server.h"

// ---- 控制台通知者: 把 SendCmd 的进度/结果打印到终端 ----
class ConsoleNotifier : public Notifier {
public:
    void notify(Role role, const std::string& name, void* param)
    {
        switch (role) {
        case Progress:
            printf("  [%s] %s\n", name.c_str(),
                   param ? (const char*)param : "");
            break;
        case Result:
            if (param) {
                Notifier::NResult* nr = (Notifier::NResult*)param;
                printf("  [result] %s: res=%d info=%s\n",
                       name.c_str(), nr->res, nr->info.c_str());
            }
            break;
        default:
            break;
        }
        fflush(stdout);
    }
};

// ---- 自测结果到达通知 (selfevalresult 命令) ----
class ConsoleSelfEvalNotifier : public SelfEvalNotifier {
public:
    void notify(const std::string& day, const std::string& user,
                const std::string& timestamp)
    {
        printf("  [selfeval-result] user=%s day=%s timestamp=%s "
               "(result dir: /home/self_evaluate/result/%s)\n",
               user.c_str(), day.c_str(), timestamp.c_str(), timestamp.c_str());
        fflush(stdout);
    }
};

static void usage(const char* prog)
{
    printf("ArbiterPlus standalone server (rebuilt)\n"
           "usage: %s [--local] [-c \"command\"] [client-ip ...]\n"
           "  --local     local self-eval mode (send only, 7729->7732 for bclient)\n"
           "  -c \"cmd\"    execute one command and exit\n", prog);
}

int main(int argc, char** argv)
{
    bool localMode = false;
    const char* oneshot = NULL;
    ClientList clients;
    bool explicitClients = false;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--local") == 0)
            localMode = true;
        else if (strcmp(argv[i], "-c") == 0 && i + 1 < argc)
            oneshot = argv[++i];
        else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            usage(argv[0]);
            return 0;
        } else {
            clients.push_back(argv[i]);
            explicitClients = true;
        }
    }
    // -c 模式: 命令串里通常已带目标(ip), 不再自动追加;
    // REPL 模式: 未显式给目标时默认 127.0.0.1
    if (!explicitClients && oneshot == NULL)
        clients.push_back("127.0.0.1");

    system("mkdir -p /var/log/arbiter");

    ConsoleSelfEvalNotifier sen;
    Server server;
    if (localMode) {
        if (!server.localServerInit("/var/log/arbiter/arbiterserver.log")) {
            fprintf(stderr, "failed to init local server (UDP 7729 in use?)\n");
            return -1;
        }
        printf("arbiter-server (local mode): UDP 7729 -> 7732, "
               "commands: version help send\n");
    } else {
        server.init("/var/log/arbiter/arbiterserver.log", &sen);
        printf("arbiter-server: UDP 7729 -> 7730, file TCP 7731, "
               "commands: version help send file mupload selfevalresult\n");
    }
    printf("default clients:");
    for (ClientList::iterator it = clients.begin(); it != clients.end(); ++it)
        printf(" %s", it->c_str());
    printf("\nlog: /var/log/arbiter/arbiterserver.log\n\n");

    ConsoleNotifier cn;

    if (oneshot != NULL) {
        printf(">>> %s\n", oneshot);
        int res = server.executeRaw(oneshot, &cn);
        printf(">>> exit code: %d\n", res);
        return res < 0 ? 1 : 0;
    }

    char line[1024];
    for (;;) {
        printf("arbiter> ");
        fflush(stdout);
        if (fgets(line, sizeof(line), stdin) == NULL)
            break;
        char* nl = strchr(line, '\n');
        if (nl) *nl = '\0';
        if (line[0] == '\0')
            continue;
        if (strcmp(line, "quit") == 0 || strcmp(line, "exit") == 0)
            break;

        int res = server.execute(&clients, line, &cn);
        printf(">>> exit code: %d\n", res);
    }
    return 0;
}
