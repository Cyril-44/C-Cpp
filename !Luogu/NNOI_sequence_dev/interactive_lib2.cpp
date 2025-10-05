/*
错误码映射（interactive_lib2.cpp）

说明：
    本文件中使用 _exit(code) 在各种错误路径上立即结束子进程或父进程。
    以下表格列出源码中显式使用的退出码及其含义、发生位置（父/子进程）与可能的触发原因和调试建议。
    注意：程序中在 waitpid 后有若干处使用 if (status) _exit(status);
             这会将 waitpid 返回的“状态字”直接作为退出码传递（并非直接的子进程退出码），
             若需要得到子进程实际退出码，应使用 WIFEXITED/WEXITSTATUS 宏解析 status。

通用/共享退出码：
    101
        含义：创建第一个管道（p_in）失败
        位置：父进程在 AliceSandbox 或 BobSandbox 的入口处
        可能原因：文件描述符耗尽、资源限制、权限问题
        调试建议：检查进程的 ulimit（nofile），检查系统调用返回的 errno

    102
        含义：创建第二个管道（p_out）失败
        位置：父进程在 AliceSandbox 或 BobSandbox 的入口处（若创建第一个管道成功）
        可能原因：同 101
        调试建议：同 101，注意在创建失败前需关闭已打开的管道端点（代码已尝试关闭）

    103
        含义：fork() 调用失败
        位置：父进程尝试 fork() 时
        可能原因：进程表已满、资源限制（RLIMIT_NPROC）、内存不足
        调试建议：检查系统限制（如 max user processes）、监控系统负载和可用内存

AliceSandbox（子进程）专用退出码：
    111
        含义：从父进程读取向量长度（uint64_t len）时失败或遇到 EOF
        位置：AliceSandbox 子进程读取输入长度
        可能原因：父进程未写入、管道被提前关闭、传输中断
        调试建议：确认父进程确实写了长度并关闭写端；在父进程中检查 xwrite 返回值与 errno

    112
        含义：从父进程读取某个 int64_t 元素失败
        位置：AliceSandbox 子进程循环读取向量元素
        可能原因：数据不完整、EOF、管道异常
        调试建议：检查父进程序列化逻辑与元素数量是否一致

    113
        含义：调用用户实现 Alice(...) 时抛出异常
        位置：AliceSandbox 子进程捕获 Alice 调用异常
        可能原因：Alice 实现中发生异常（例如越界、抛出 runtime_error 等）
        调试建议：在 Alice 实现中添加异常处理或本地日志以定位异常原因

    114
        含义：向父进程写回 bytes（uint64_t）失败
        位置：AliceSandbox 子进程在发送输出长度时写失败
        可能原因：管道被关闭、写端出现错误
        调试建议：检查父进程是否已提前关闭读端或出现阻塞/错误

    115
        含义：向父进程写回字符串内容失败
        位置：AliceSandbox 子进程在发送输出字节时写失败
        可能原因：同 114
        调试建议：同 114

BobSandbox（子进程）专用退出码：
    121
        含义：读取 int32_t n_in 失败或 EOF
        位置：BobSandbox 子进程读取 n 参数
        可能原因：父进程未发送 n、管道被关闭
        调试建议：检查父进程调用 BobSandbox 时是否正确写入 n（xwrite 返回值）

    122
        含义：读取 uint64_t slen（字符串长度）失败或 EOF
        位置：BobSandbox 子进程读取字符串长度
        可能原因：父进程序列化字符串长度时出错
        调试建议：检查父进程写入 slen 的代码路径

    123
        含义：读取字符串体（in_s）失败（未读满 slen 字节）
        位置：BobSandbox 子进程读取字符串内容
        可能原因：数据不完整或管道提前关闭
        调试建议：验证父进程写入了与 slen 相同长度的数据

    124
        含义：调用用户实现 Bob(n, s) 时抛出异常
        位置：BobSandbox 子进程捕获 Bob 调用异常
        可能原因：Bob 实现内部异常（越界、非法访问等）
        调试建议：在 Bob 实现中添加异常处理或本地日志以定位异常原因

    125
        含义：向父进程写回结果向量长度（uint64_t len）失败
        位置：BobSandbox 子进程在序列化返回向量长度时写失败
        可能原因：管道出错或父进程读端已关闭
        调试建议：检查父进程是否仍然在等待读取；查看 xwrite 返回值与 errno

    126
        含义：向父进程写回某个 int64_t 元素失败
        位置：BobSandbox 子进程在序列化返回向量元素时写失败
        可能原因：同 125
        调试建议：同 125

父进程（BobSandbox 读取返回时）专用退出码：
    109
        含义：父进程从子进程读回 len（uint64_t）失败 —— BobSandbox 的父端读取结果失败
        位置：BobSandbox 父进程在读取子进程返回的向量长度时
        可能原因：子进程异常退出、管道被关闭、传输中断
        调试建议：在父进程中检查子进程状态（waitpid 返回的 status）；查看子进程 stdout/stderr（如有）/日志

关于 waitpid 后的 status 传递：
    在多个父进程分支末尾，代码使用：
        int status = 0; waitpid(pid, &status, 0);
        if (status) _exit(status);
    这会将 waitpid 填充的“状态字”直接作为退出码返回；该状态字包含了信号终止、退出码等信息，
    并非直接的 0..255 的子进程退出码。若上层需要解释真实的子进程退出码，应改为：
        if (WIFEXITED(status)) exit_code = WEXITSTATUS(status);
        else if (WIFSIGNALED(status)) // 处理被信号终止的情况 
    否则直接将 status 作为进程退出码可能导致难以理解的值。

调试建议（通用）：
    - 在发生 _exit(...) 时，若要确定原因，优先查看子进程/父进程的 errno 值及系统日志。
    - 在开发阶段可将子进程内部的 _exit(...) 替换为写入调试信息（例如写到父进程的另一个管道或临时文件），便于定位。
    - 注意管道读写必须严格匹配写入的数据格式与长度（uint64_t len，随后 len 个 int64_t，或 n + uint64_t slen + slen 字节）。
    - 对于 Alice 和 Bob 的用户实现（Alice(...)/Bob(...)），建议捕获并记录异常，避免孩子进程因未捕获异常而用 113/124 等码退出。

简要一览（退出码 -> 简短含义）：
    101: 创建 p_in 管道失败
    102: 创建 p_out 管道失败（在创建 p_in 成功后）
    103: fork() 失败
    109: 父进程读取 Bob 子进程返回的 len 失败（BobSandbox 父端）
    111: Alice 子进程读取输入 len 失败
    112: Alice 子进程读取某元素失败
    113: Alice() 抛出异常
    114: Alice 子进程写入返回长度失败
    115: Alice 子进程写入返回数据失败
    121: Bob 子进程读取 n 失败
    122: Bob 子进程读取 slen 失败
    123: Bob 子进程读取字符串体失败
    124: Bob() 抛出异常
    125: Bob 子进程写入返回长度失败
    126: Bob 子进程写入某返回元素失败

结束语：
    本映射旨在帮助定位子进程/父进程在 IPC（基于管道）和序列化交互中的失败点。
    若需进一步定位，建议结合 errno、strace 跟踪（注意 seccomp 可能影响可用系统调用）和在用户实现中添加更多诊断信息。
*/
#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/prctl.h>
#include <sys/syscall.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/seccomp.h>
#include <linux/filter.h>
#include <linux/audit.h>
#include <linux/capability.h>
#include <errno.h>
#include <grp.h>

std::string Alice(std::vector<long long> a);

std::vector<long long> Bob(int n, std::string s);

/* --------------------------------------------------------------------------
   === Sandbox utilities ===
   - apply_sandbox(): set no_new_privs, drop capabilities, install seccomp BPF
     to kill on a list of blacklisted syscalls (memfd/shm/mq/execve/...).
   - For portability, every blocked syscall is wrapped by #ifdef __NR_...
   --------------------------------------------------------------------------*/

static bool install_simple_seccomp_blacklist(const std::vector<int> &blacklist) {
    // Build a tiny BPF program:
    // if (nr == X) return KILL;
    // ...
    // return ALLOW;
    std::vector<sock_filter> filt;

    // load syscall number
    filt.push_back(BPF_STMT(BPF_LD | BPF_W | BPF_ABS, offsetof(struct seccomp_data, nr)));

    for (int nr : blacklist) {
        // jump if equal -> then return KILL
        filt.push_back((sock_filter)BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, nr, 0, 1));
        filt.push_back(BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_KILL_PROCESS));
    }
    // default allow
    filt.push_back(BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_ALLOW));

    struct sock_fprog prog;
    prog.len = static_cast<unsigned short>(filt.size());
    prog.filter = filt.data();

    if (prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) != 0) {
        // If we can't set NO_NEW_PRIVS, fail the sandbox installation
        return false;
    }
    if (prctl(PR_SET_SECCOMP, SECCOMP_MODE_FILTER, &prog) != 0) {
        return false;
    }
    return true;
}

static void drop_all_capabilities() {
#ifdef PR_CAPBSET_DROP
    // Try to drop capability bitset entries (best-effort)
    for (int c = 0; c <= CAP_LAST_CAP; ++c) {
        // ignore errors
        prctl(PR_CAPBSET_DROP, c, 0, 0, 0);
    }
#endif
    // Also set the process to not be dumpable
    prctl(PR_SET_DUMPABLE, 0, 0, 0, 0);
    // Clear supplementary groups
    setgroups(0, nullptr);
    // Set real/effective/saved UID/GID to the current real ones (clear saved-set)
    uid_t ruid = getuid(), rgid = getgid();
    setresgid(rgid, rgid, rgid);
    setresuid(ruid, ruid, ruid);
}

static bool apply_sandbox() {
    // Best-effort sandbox. Returns true if seccomp installed successfully.
    // Drop caps and prevent new privileges.
    drop_all_capabilities();

    // Build a blacklist of syscalls we definitely don't want child to be able to call.
    // We intentionally block a short list: execve/execveat, memfd_create, shm*, mq*.
    // List of system call numbers to block (no #ifdef required)
    static const std::vector<int> black = {
        __NR_execve,
        __NR_execveat,
        __NR_memfd_create,
        __NR_shmget,
        __NR_shmat,
        __NR_shmctl,
        __NR_shmdt,
        __NR_mq_open,
        __NR_mq_unlink,
        __NR_mq_timedsend,
        __NR_mq_timedreceive,
        __NR_mq_notify,
        __NR_mq_getsetattr,
        __NR_socketpair,
        __NR_socket,
        __NR_connect,
        __NR_accept,
        __NR_bind,
        __NR_listen,
        __NR_sendto,
        __NR_recvfrom,
        __NR_sendmsg,
        __NR_recvmsg
    };

    // Install the seccomp filter (best-effort).
    bool ok = install_simple_seccomp_blacklist(black);
    return ok;
}

/* --------------------------------------------------------------------------
   === IPC helpers: binary simple protocol ===
   - Parent -> child (for Alice): write uint64_t len then len x int64_t values.
   - Child reads them and calls Alice.
   - Child -> parent: write uint64_t bytes length then byte data (string body).
   - For Bob: Parent sends (int n) + string s (length prefixed).
   --------------------------------------------------------------------------*/

static ssize_t xwrite(int fd, const void *buf, size_t count) {
    const char *p = (const char*)buf;
    size_t left = count;
    while (left) {
        ssize_t w = ::write(fd, p, left);
        if (w < 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        left -= (size_t)w;
        p += w;
    }
    return (ssize_t)count;
}

static ssize_t xread_full(int fd, void *buf, size_t count) {
    char *p = (char*)buf;
    size_t left = count;
    while (left) {
        ssize_t r = ::read(fd, p, left);
        if (r == 0) return (ssize_t)(count - left); // EOF
        if (r < 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        left -= (size_t)r;
        p += r;
    }
    return (ssize_t)count;
}

/* AliceSandbox:
    - parent serializes vector<long long> a to child
    - child deserializes, calls Alice, writes resulting string back
    - parent returns string (no validation)
*/
static std::string AliceSandbox(std::vector<long long> a) {
    int p_in[2];  // parent -> child
    int p_out[2]; // child -> parent
    if (pipe(p_in) != 0) {
        _exit(101);
    }
    if (pipe(p_out) != 0) {
        close(p_in[0]); close(p_in[1]);
        _exit(102);
    }

    pid_t pid = fork();
    if (pid < 0) {
        // fork failed
        close(p_in[0]); close(p_in[1]); close(p_out[0]); close(p_out[1]);
        _exit(103);
    } else if (pid == 0) {
        // CHILD
        // close parent's ends
        close(p_in[1]);
        close(p_out[0]);

        // apply sandbox (best-effort). If it fails we still continue (best-effort).
        apply_sandbox();

        // Read vector<long long> from p_in[0]
        uint64_t len = 0;
        if (xread_full(p_in[0], &len, sizeof(len)) != (ssize_t)sizeof(len)) {
            // read error/EOF -> exit child
            _exit(111);
        }
        std::vector<long long> vec;
        vec.resize(len);
        for (uint64_t i = 0; i < len; ++i) {
            int64_t v = 0;
            if (xread_full(p_in[0], &v, sizeof(v)) != (ssize_t)sizeof(v)) _exit(112);
            vec[i] = (long long)v;
        }
        // Call user implementation
        std::string out;
        try {
            out = Alice(std::move(vec));
        } catch (...) {
            // swallow exceptions inside child
            _exit(113);
        }
        // send back
        uint64_t bytes = out.size();
        if (xwrite(p_out[1], &bytes, sizeof(bytes)) < 0) _exit(114);
        if (bytes) {
            if (xwrite(p_out[1], out.data(), out.size()) < 0) _exit(115);
        }
        // clean exit
        _exit(0);
    } else {
        // PARENT
        close(p_in[0]);
        close(p_out[1]);

        // serialize vector<long long> to child
        uint64_t len = a.size();
        if (xwrite(p_in[1], &len, sizeof(len)) < 0) {
            // write error
        } else {
            for (uint64_t i = 0; i < len; ++i) {
                int64_t v = (int64_t)a[i];
                xwrite(p_in[1], &v, sizeof(v));
            }
        }
        close(p_in[1]); // EOF to child

        // read result
        uint64_t bytes = 0;
        ssize_t got = xread_full(p_out[0], &bytes, sizeof(bytes));
        std::string out;
        if (got == (ssize_t)sizeof(bytes) && bytes > 0) {
            out.resize(bytes);
            ssize_t r = xread_full(p_out[0], &out[0], bytes);
            if (r < 0) out.clear();
        } else {
            // no bytes or read error -> return empty string
        }

        close(p_out[0]);
        // wait for child (reap)
        int status = 0;
        waitpid(pid, &status, 0);
        if (status) _exit(status);
        return out;
    }
}

/* BobSandbox:
    - parent sends int n and string data to child
    - child reads, calls Bob(n, s), serializes vector<long long> back
*/
static std::vector<long long> BobSandbox(int n, std::string s) {
    int p_in[2];
    int p_out[2];
    if (pipe(p_in) != 0) _exit(101);
    if (pipe(p_out) != 0) { close(p_in[0]); close(p_in[1]); _exit(102); }

    pid_t pid = fork();
    if (pid < 0) {
        close(p_in[0]); close(p_in[1]); close(p_out[0]); close(p_out[1]);
        _exit(103);
    } else if (pid == 0) {
        // child
        close(p_in[1]); close(p_out[0]);
        apply_sandbox();

        // read n (int) and string length prefixed
        int32_t n_in = 0;
        if (xread_full(p_in[0], &n_in, sizeof(n_in)) != (ssize_t)sizeof(n_in)) _exit(121);
        uint64_t slen = 0;
        if (xread_full(p_in[0], &slen, sizeof(slen)) != (ssize_t)sizeof(slen)) _exit(122);
        std::string in_s;
        if (slen) {
            in_s.resize(slen);
            if (xread_full(p_in[0], &in_s[0], slen) != (ssize_t)slen) _exit(123);
        }

        // call Bob
        std::vector<long long> res;
        try {
            res = Bob(n_in, in_s);
        } catch (...) {
            _exit(124);
        }

        // serialize vector<long long> back
        uint64_t len = res.size();
        if (xwrite(p_out[1], &len, sizeof(len)) < 0) _exit(125);
        for (uint64_t i = 0; i < len; ++i) {
            int64_t v = (int64_t)res[i];
            if (xwrite(p_out[1], &v, sizeof(v)) < 0) _exit(126);
        }
        _exit(0);
    } else {
        // parent
        close(p_in[0]); close(p_out[1]);

        int32_t n32 = n;
        xwrite(p_in[1], &n32, sizeof(n32));
        uint64_t slen = s.size();
        xwrite(p_in[1], &slen, sizeof(slen));
        if (slen) xwrite(p_in[1], s.data(), slen);
        close(p_in[1]);

        // read back
        uint64_t len = 0;
        if (xread_full(p_out[0], &len, sizeof(len)) != (ssize_t)sizeof(len)) {
            close(p_out[0]);
            int status = 0; waitpid(pid, &status, 0);
            _exit(109);
        }
        std::vector<long long> res;
        res.resize(len);
        for (uint64_t i = 0; i < len; ++i) {
            int64_t v = 0;
            if (xread_full(p_out[0], &v, sizeof(v)) != (ssize_t)sizeof(v)) {
                res.resize(i);
                break;
            }
            res[i] = (long long)v;
        }
        close(p_out[0]);
        int status = 0; waitpid(pid, &status, 0);
        if (status) _exit(status);
        return res;
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    int n;
    std::cin >> n;
    std::vector<long long> a(n);
    for (int i = 0; i < n; i++) std::cin >> a[i];

    std::string data = AliceSandbox(std::move(a));
    std::cout << data << std::endl;

    std::vector<long long> ans = BobSandbox(n, std::move(data));
    std::cout << ans.size();
    for (long long i : ans) std::cout << ' ' << i;
    std::cout << std::endl;
    return 0;
}
