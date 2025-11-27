// interactive_lib_sandbox_noseccmp.cpp
// Single-file C++17 interactive harness that runs user-provided Alice(vector<int>)
// and Bob(int, string) functions in sandboxed child processes to reduce
// chance of illicit communication between them while avoiding seccomp (for OJ compatibility).
//
// NOTE:
// - This version removes seccomp installation (which can cause SIGABRT / RE on some OJs).
// - It still applies resource limits, clears environment with clearenv(), closes most FDs,
//   redirects stdio to /dev/null in children, and enforces wall-clock timeout by parent kill.
// - Compile with: g++ -std=c++17 interactive_lib_sandbox_noseccmp.cpp user_submission.cpp -O2 -o judge
//
// The judge harness protocol for communication is:
//  - Child running Alice writes: uint64_t len followed by len bytes (the returned string).
//  - Child running Bob writes: uint64_t count followed by count of int64_t values (the returned vector).
//
// Input format (to this harness): t tests; for each test: n then n integers.
// Output: either error indicators or Bob's returned vector printed as: cnt v0 v1 ... v(cnt-1)
#define _GNU_SOURCE
#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <poll.h>
#include <sys/prctl.h>
#include <mqueue.h>
#include <sys/mman.h>  // shm_open 的声明
using namespace std;

#define _SYSCALL_H	1

extern "C" {

// memfd_create（Linux 2.27+ / glibc 2.27+）
int memfd_create(const char* /*name*/, unsigned int /*flags*/) {
    errno = EACCES;
    return -1;
}

// POSIX 共享内存
int shm_open(const char* /*name*/, int /*flags*/, mode_t /*mode*/) {
    errno = EACCES;
    return -1;
}

// POSIX mqueue
mqd_t _tm_mq_open(const char* /*name*/,
              int /*flags*/,
              mode_t /*mode*/,
              struct mq_attr* /*attr*/) 
{
    errno = EACCES;
    return (mqd_t)-1;
}

#define mq_open _tm_mq_open

int mq_send(mqd_t /*mqdes*/,
            const char* /*msg_ptr*/,
            size_t /*msg_len*/,
            unsigned /*msg_prio*/)
{
    errno = EACCES;
    return -1;
}

ssize_t mq_receive(mqd_t /*mqdes*/,
                   char* /*msg_ptr*/,
                   size_t /*msg_len*/,
                   unsigned* /*msg_prio*/)
{
    errno = EACCES;
    return -1;
}

// System V 消息队列
int msgget(key_t /*key*/, int /*msgflg*/) {
    errno = EACCES;
    return -1;
}

int msgsnd(int /*msqid*/, const void* /*msgp*/, size_t /*msgsz*/, int /*msgflg*/) {
    errno = EACCES;
    return -1;
}

ssize_t msgrcv(int /*msqid*/,
               void* /*msgp*/,
               size_t /*msgsz*/,
               long /*msgtyp*/,
               int /*msgflg*/)
{
    errno = EACCES;
    return -1;
}

}

// Contestant must provide these two functions in their submission file:
std::string Alice(std::vector<int>);
std::vector<int> Bob(int, std::string);

// --- I/O helpers
static bool write_all(int fd, const void* buf, size_t sz) {
    const char* p = (const char*)buf;
    while (sz) {
        ssize_t w = write(fd, p, sz);
        if (w < 0) {
            if (errno == EINTR) continue;
            return false;
        }
        p += w;
        sz -= (size_t)w;
    }
    return true;
}
static bool read_all(int fd, void* buf, size_t sz) {
    char* p = (char*)buf;
    while (sz) {
        ssize_t r = read(fd, p, sz);
        if (r < 0) {
            if (errno == EINTR) continue;
            return false;
        }
        if (r == 0) return false;
        p += r;
        sz -= (size_t)r;
    }
    return true;
}

// Close all file descriptors >= 3 except those in keep vector.
static void close_all_fds_except(const vector<int>& keep) {
    int maxfd = (int)sysconf(_SC_OPEN_MAX);
    if (maxfd < 0) maxfd = 1024;
    unordered_set<int> ks(keep.begin(), keep.end());
    for (int fd = 3; fd < maxfd; ++fd) {
        if (ks.find(fd) == ks.end()) close(fd);
    }
}

// Setup child sandbox basics: rlimits, clear environment, close fds (caller must ensure pipe fd is kept)
static void setup_child_sandbox_basic(const vector<int>& keep_fds) {
    prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0);
    // Clear environment (POSIX)
#if defined(_GNU_SOURCE) || defined(__linux__)
    // clearenv is typically available on Linux
    clearenv();
#else
    extern char** environ;
    environ = nullptr;
#endif

    // Resource limits
    struct rlimit rl;
    // No core
    rl.rlim_cur = rl.rlim_max = 0;
    setrlimit(RLIMIT_CORE, &rl);
    // CPU seconds (soft) - small
    rl.rlim_cur = rl.rlim_max = 2; // seconds of CPU
    setrlimit(RLIMIT_CPU, &rl);
    // Address space limit (~512MB)
    rl.rlim_cur = rl.rlim_max = 512ULL * 1024ULL * 1024ULL;
    setrlimit(RLIMIT_AS, &rl);
    // Limit number of files
    rl.rlim_cur = rl.rlim_max = 16;
    setrlimit(RLIMIT_NOFILE, &rl);
    // File size 0
    rl.rlim_cur = rl.rlim_max = 0;
    setrlimit(RLIMIT_FSIZE, &rl);

    // Restrictive umask
    umask(077);

    // Close file descriptors except those in keep_fds
    close_all_fds_except(keep_fds);
}

// Wait for pid with wall-clock timeout in seconds; kills pid if timeout.
// Returns child's status (as from waitpid) or -1 on error.
static int waitpid_with_timeout(pid_t pid, int wall_seconds) {
    const useconds_t interval_us = 10000; // 10ms
    int elapsed_us = 0;
    while (true) {
        int status = 0;
        pid_t r = waitpid(pid, &status, WNOHANG);
        if (r == pid) return status;
        if (r == -1) return -1;
        usleep(interval_us);
        elapsed_us += interval_us;
        if (elapsed_us >= wall_seconds * 1000000) {
            // timeout
            kill(pid, SIGKILL);
            waitpid(pid, &status, 0);
            return status;
        }
    }
}

static bool read_exact_with_timeout(int fd, void* buf, size_t sz, int timeout_ms) {
    uint8_t* p = (uint8_t*)buf;
    size_t left = sz;
    while (left > 0) {
        struct pollfd pfd{fd, POLLIN, 0};
        int rc = poll(&pfd, 1, timeout_ms);
        if (rc == 0) return false; // timeout
        if (rc < 0) { if (errno == EINTR) continue; return false; }
        if (pfd.revents & (POLLERR | POLLHUP)) {
            // 仍可能有剩余数据可读，先尝试一次 read
        }
        ssize_t r = read(fd, p, left);
        if (r < 0) { if (errno == EINTR) continue; if (errno == EAGAIN) continue; return false; }
        if (r == 0) return false; // EOF before sz
        p += r;
        left -= (size_t)r;
        // 可根据需要缩短剩余 timeout_ms（略）
    }
    return true;
}

static bool read_upto_with_timeout(int fd, std::string& dst, uint64_t total, int timeout_ms) {
    const size_t BUF = 8192;
    dst.resize((size_t)total);
    size_t off = 0;
    while (off < total) {
        struct pollfd pfd{fd, POLLIN, 0};
        int rc = poll(&pfd, 1, timeout_ms);
        if (rc == 0) return false;
        if (rc < 0) { if (errno == EINTR) continue; return false; }
        ssize_t r = read(fd, &dst[off], (size_t)std::min<uint64_t>(BUF, total - off));
        if (r < 0) { if (errno == EINTR) continue; if (errno == EAGAIN) continue; return false; }
        if (r == 0) return false; // EOF early → truncated
        off += (size_t)r;
        // 同样可递减 timeout_ms（略）
    }
    return true;
}


// Run Alice in sandbox child, fetch returned string.
// Protocol: write uint64_t len then len bytes.
// On exception, child writes len == (uint64_t)-1, then uint64_t msglen and message.
static bool run_alice_sandbox(const vector<int>& a, string& out_s, string& err_msg, int wall_timeout_sec = 3) {
    int pipefd[2];
    if (pipe(pipefd) != 0) { return false; }
    pid_t pid = fork();
    if (pid < 0) { close(pipefd[0]); close(pipefd[1]); err_msg = "fork failed"; return false; }
    if (pid == 0) {
        // Child
        // ensure write end is on fd 3
        int wfd = pipefd[1];
        if (wfd != 3) {
            if (dup2(wfd, 3) < 0) _exit(127);
            close(wfd);
        }
        // Redirect stdout/stderr to /dev/null to avoid leaking output
        int dn = open("/dev/null", O_RDWR);
        if (dn >= 0) {
            dup2(dn, STDOUT_FILENO);
            dup2(dn, STDERR_FILENO);
            if (dn > 2) close(dn);
        }
        // Keep fds 0,1,2,3
        vector<int> keep = {0,1,2,3};
        setup_child_sandbox_basic(keep);

        try {
            string s = Alice(a);
            uint64_t len = (uint64_t)s.size();
            if (!write_all(3, &len, sizeof(len))) { _exit(1); }
            if (len) {
                if (!write_all(3, s.data(), s.size())) { _exit(1); }
            }
        } catch (const std::exception& ex) {
            uint64_t sentinel = (uint64_t)-1;
            write_all(3, &sentinel, sizeof(sentinel));
            string msg = ex.what();
            uint64_t mlen = (uint64_t)msg.size();
            write_all(3, &mlen, sizeof(mlen));
            if (mlen) write_all(3, msg.data(), mlen);
            _exit(1);
        } catch (...) {
            uint64_t sentinel = (uint64_t)-1;
            write_all(3, &sentinel, sizeof(sentinel));
            uint64_t mlen = 0;
            write_all(3, &mlen, sizeof(mlen));
            _exit(1);
        }
        _exit(0);
    } else {
        // Parent
        close(pipefd[1]);

        // 可选：将 pipefd[0] 设为 O_NONBLOCK，poll 循环更顺滑
        int flags = fcntl(pipefd[0], F_GETFL, 0);
        fcntl(pipefd[0], F_SETFL, flags | O_NONBLOCK);

        const int total_timeout_ms = wall_timeout_sec * 1000;
        auto t0 = std::chrono::steady_clock::now();

        auto elapsed_ms = [&]{
            return (int)std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - t0).count();
        };

        uint64_t len = 0;
        if (!read_exact_with_timeout(pipefd[0], &len, sizeof(len),
                                    total_timeout_ms - elapsed_ms())) {
            // 如果子进程已经退出，给出更友好的诊断
            int status = 0;
            pid_t w = waitpid(pid, &status, WNOHANG);
            if (w == pid) {
                if (WIFSIGNALED(status)) err_msg = std::string("Alice child signaled ") + std::to_string(WTERMSIG(status));
                else if (WIFEXITED(status)) err_msg = std::string("Alice child exited code ") + std::to_string(WEXITSTATUS(status));
                else err_msg = "Alice child abnormal exit";
            } else {
                err_msg = "Alice header read timeout or failed";
            }
            close(pipefd[0]);
            return false;
        }

        if (len == (uint64_t)-1) {
            uint64_t mlen = 0;
            if (!read_exact_with_timeout(pipefd[0], &mlen, sizeof(mlen),
                                        total_timeout_ms - elapsed_ms())) {
                err_msg = "Alice error msg truncated";
                close(pipefd[0]);
                (void)waitpid(pid, nullptr, 0);
                return false;
            }
            std::string msg;
            if (mlen) {
                msg.resize((size_t)mlen);
                if (!read_upto_with_timeout(pipefd[0], msg, mlen,
                                            total_timeout_ms - elapsed_ms())) {
                    err_msg = "Alice error msg truncated";
                    close(pipefd[0]);
                    (void)waitpid(pid, nullptr, 0);
                    return false;
                }
            }
            err_msg = std::string("Alice threw: ") + msg;
            close(pipefd[0]);
            (void)waitpid(pid, nullptr, 0);
            return false;
        }

        if (len > 1000000ULL) {
            // 按原意 drain 并报错，只是用非阻塞+poll，避免卡住
            uint64_t toread = len;
            const size_t BUF = 8192;
            char tmp[BUF];
            while (toread) {
                struct pollfd pfd{pipefd[0], POLLIN, 0};
                int rc = poll(&pfd, 1, std::max(1, total_timeout_ms - elapsed_ms()));
                if (rc <= 0) break;
                ssize_t rr = read(pipefd[0], tmp, (size_t)std::min<uint64_t>(BUF, toread));
                if (rr <= 0) break;
                toread -= (uint64_t)rr;
            }
            err_msg = "Alice returned too large string";
            close(pipefd[0]);
            (void)waitpid(pid, nullptr, 0);
            return false;
        }

        // 读取正文（与子并发，避免死锁）
        std::string s;
        if (len) {
            if (!read_upto_with_timeout(pipefd[0], s, len,
                                        total_timeout_ms - elapsed_ms())) {
                err_msg = "Alice output truncated";
                close(pipefd[0]);
                (void)waitpid(pid, nullptr, 0);
                return false;
            }
        }
        close(pipefd[0]);

        // 最后再等待子进程退出（有剩余时间的话）
        int remain = total_timeout_ms - elapsed_ms();
        if (remain < 0) remain = 0;
        int status = 0;
        pid_t w = 0;
        /* do {
            w = waitpid(pid, &status, remain ? WNOHANG : 0);
            if (w == 0) {
                // 还活着且超时 → 杀掉
                kill(pid, SIGKILL);
                (void)waitpid(pid, &status, 0);
                err_msg = "Alice child timeout";
                return false;
            }
        } while (w < 0 && errno == EINTR); */

        out_s = std::move(s);
        return true;
    }
}

using clock_ = std::chrono::steady_clock;

static inline int remaining_ms(clock_::time_point deadline) {
    auto now = clock_::now();
    if (now >= deadline) return 0;
    return (int)std::chrono::duration_cast<std::chrono::milliseconds>(deadline - now).count();
}

static bool read_exact_poll(int fd, void* buf, size_t sz, clock_::time_point deadline) {
    uint8_t* p = (uint8_t*)buf;
    size_t left = sz;
    while (left > 0) {
        int to = remaining_ms(deadline);
        if (to <= 0) return false; // timeout
        struct pollfd pfd{fd, POLLIN, 0};
        int rc = poll(&pfd, 1, to);
        if (rc == 0) return false; // timeout
        if (rc < 0) { if (errno == EINTR) continue; return false; }
        // 尽量多读（配合 O_NONBLOCK）
        ssize_t r = read(fd, p, left);
        if (r > 0) { p += r; left -= (size_t)r; continue; }
        if (r == 0) return false; // EOF
        if (errno == EINTR || errno == EAGAIN) continue;
        return false;
    }
    return true;
}

static bool drain_bytes_poll(int fd, uint64_t bytes, clock_::time_point deadline) {
    // 安全上限：最多尝试到 deadline；读不到也不阻塞
    constexpr size_t BUF = 64 * 1024;
    char buf[BUF];
    uint64_t left = bytes;
    while (left > 0) {
        int to = remaining_ms(deadline);
        if (to <= 0) return false; // timeout
        struct pollfd pfd{fd, POLLIN, 0};
        int rc = poll(&pfd, 1, to);
        if (rc == 0) return false;
        if (rc < 0) { if (errno == EINTR) continue; return false; }
        for (;;) {
            size_t want = (size_t)std::min<uint64_t>(left, BUF);
            ssize_t r = read(fd, buf, want);
            if (r > 0) { left -= (uint64_t)r; }
            else if (r == 0) return false; // EOF before expected
            else if (errno == EINTR) continue;
            else if (errno == EAGAIN) break; // 等下一轮 poll
            else return false;
            if (left == 0) break;
            // 若还有数据，尝试继续读直到 EAGAIN
        }
    }
    return true;
}

static bool read_i64_vector_poll(int fd, uint64_t cnt, std::vector<int>& out_v, clock_::time_point deadline) {
    // 流式解析：读到内存块后按 8 字节切分为 int64_t
    constexpr size_t CHUNK = 64 * 1024;
    std::vector<char> acc;
    acc.reserve(CHUNK);
    size_t pos = 0;

    out_v.clear();
    out_v.reserve((size_t)std::min<uint64_t>(cnt, 1000000ULL)); // 与原逻辑一致

    char tmp[CHUNK];
    while (out_v.size() < cnt) {
        // 先尽可能解析已有缓冲
        while (acc.size() - pos >= sizeof(int64_t) && out_v.size() < cnt) {
            int64_t v;
            std::memcpy(&v, acc.data() + pos, sizeof(int64_t));
            pos += sizeof(int64_t);
            out_v.push_back((int)v); // 若需溢出检查，可在此加入范围校验
        }
        if (out_v.size() >= cnt) break;

        // 压缩缓冲（避免 acc 无限增长）
        if (pos > 0 && (pos > 4096 || pos > acc.size() / 2)) {
            std::memmove(acc.data(), acc.data() + pos, acc.size() - pos);
            acc.resize(acc.size() - pos);
            pos = 0;
        }

        int to = remaining_ms(deadline);
        if (to <= 0) return false; // timeout
        struct pollfd pfd{fd, POLLIN, 0};
        int rc = poll(&pfd, 1, to);
        if (rc == 0) return false; // timeout
        if (rc < 0) { if (errno == EINTR) continue; return false; }

        // 尝试读尽可能多的数据直到 EAGAIN
        for (;;) {
            ssize_t r = read(fd, tmp, CHUNK);
            if (r > 0) {
                size_t old = acc.size();
                acc.resize(old + (size_t)r);
                std::memcpy(acc.data() + old, tmp, (size_t)r);
            } else if (r == 0) {
                // 子提前关闭且不足一个完整元素
                return false;
            } else if (errno == EINTR) {
                continue;
            } else if (errno == EAGAIN) {
                break; // 等下一轮 poll
            } else {
                return false;
            }
        }
    }
    return true;
}

static bool run_bob_sandbox(int n, const std::string& s, std::vector<int>& out_v, std::string& err_msg, int wall_timeout_sec = 3) {
    int pipefd[2];
    if (pipe(pipefd) != 0) { err_msg = "pipe failed"; return false; }
    pid_t pid = fork();
    if (pid < 0) { close(pipefd[0]); close(pipefd[1]); err_msg = "fork failed"; return false; }
    if (pid == 0) {
        // Child
        int wfd = pipefd[1];
        if (wfd != 3) {
            if (dup2(wfd, 3) < 0) _exit(127);
            close(wfd);
        }
        int dn = open("/dev/null", O_RDWR);
        if (dn >= 0) {
            dup2(dn, STDOUT_FILENO);
            dup2(dn, STDERR_FILENO);
            if (dn > 2) close(dn);
        }
        std::vector<int> keep = {0,1,2,3};
        setup_child_sandbox_basic(keep);

        try {
            std::vector<int> res = Bob(n, s);
            uint64_t cnt = (uint64_t)res.size();
            if (!write_all(3, &cnt, sizeof(cnt))) _exit(1);
            for (size_t i = 0; i < res.size(); ++i) {
                int64_t v = (int64_t)res[i];
                if (!write_all(3, &v, sizeof(v))) _exit(1);
            }
        } catch (const std::exception& ex) {
            uint64_t sentinel = (uint64_t)-1;
            write_all(3, &sentinel, sizeof(sentinel));
            std::string msg = ex.what();
            uint64_t mlen = (uint64_t)msg.size();
            write_all(3, &mlen, sizeof(mlen));
            if (mlen) write_all(3, msg.data(), mlen);
            _exit(1);
        } catch (...) {
            uint64_t sentinel = (uint64_t)-1;
            write_all(3, &sentinel, sizeof(sentinel));
            uint64_t mlen = 0;
            write_all(3, &mlen, sizeof(mlen));
            _exit(1);
        }
        _exit(0);
    } else {
        // Parent: 读取优先，等待在后
        close(pipefd[1]);

        // 非阻塞 + poll
        int flags = fcntl(pipefd[0], F_GETFL, 0);
        fcntl(pipefd[0], F_SETFL, flags | O_NONBLOCK);

        auto deadline = clock_::now() + std::chrono::seconds(wall_timeout_sec);

        uint64_t cnt = 0;
        if (!read_exact_poll(pipefd[0], &cnt, sizeof(cnt), deadline)) {
            // 看看子是否已退出，便于更友好的错误
            int status = 0;
            pid_t w = waitpid(pid, &status, WNOHANG);
            if (w == pid) {
                if (WIFSIGNALED(status)) err_msg = std::string("Bob child signaled ") + std::to_string(WTERMSIG(status));
                else if (WIFEXITED(status)) err_msg = std::string("Bob child exited code ") + std::to_string(WEXITSTATUS(status));
                else err_msg = "Bob child abnormal exit";
            } else {
                err_msg = "Bob header read timeout or failed";
            }
            close(pipefd[0]);
            // 收尸
            (void)waitpid(pid, nullptr, 0);
            return false;
        }

        if (cnt == (uint64_t)-1) {
            uint64_t mlen = 0;
            if (!read_exact_poll(pipefd[0], &mlen, sizeof(mlen), deadline)) {
                err_msg = "Bob error msg truncated";
                close(pipefd[0]);
                (void)waitpid(pid, nullptr, 0);
                return false;
            }
            std::string msg;
            if (mlen) {
                msg.resize((size_t)mlen);
                if (!read_exact_poll(pipefd[0], &msg[0], (size_t)mlen, deadline)) {
                    err_msg = "Bob error msg truncated";
                    close(pipefd[0]);
                    (void)waitpid(pid, nullptr, 0);
                    return false;
                }
            }
            err_msg = std::string("Bob threw: ") + msg;
            close(pipefd[0]);
            (void)waitpid(pid, nullptr, 0);
            return false;
        }

        if (cnt > 2000000ULL) {
            // 过大：报错并尽量 drain，避免子阻塞在写 pipe
            err_msg = "Bob returned too many elements";
            // 注意：总字节数可能很大，按 deadline 限制 drain 时间
            __uint128_t bytes128 = (__uint128_t)cnt * (__uint128_t)sizeof(int64_t);
            uint64_t bytes = (bytes128 > (__uint128_t)UINT64_MAX) ? UINT64_MAX : (uint64_t)bytes128;
            (void)drain_bytes_poll(pipefd[0], bytes, deadline);
            close(pipefd[0]);
            (void)waitpid(pid, nullptr, 0);
            return false;
        }

        // 正常读取向量
        if (cnt > 0) {
            if (!read_i64_vector_poll(pipefd[0], cnt, out_v, deadline)) {
                err_msg = "Bob output truncated";
                close(pipefd[0]);
                (void)waitpid(pid, nullptr, 0);
                return false;
            }
        } else {
            out_v.clear();
        }

        close(pipefd[0]);

        // 最后等待子进程退出（在剩余时间内）
        int status = 0;
        for (;;) {
            pid_t w = waitpid(pid, &status, WNOHANG);
            if (w == pid) break;
            if (w < 0 && errno != EINTR) break;
            int to = remaining_ms(deadline);
            if (to <= 0) {
                kill(pid, SIGKILL);
                (void)waitpid(pid, &status, 0);
                err_msg = "Bob child timeout";
                return false;
            }
            // 小睡 1ms，避免忙等
            struct timespec ts{0, 1000000};
            nanosleep(&ts, nullptr);
        }

        return true;
    }
}

// Main harness
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    if (!(cin >> t)) return 0;
    for (int tc = 0; tc < t; ++tc) {
        int n;
        cin >> n;
        vector<int> a(n);
        for (int i = 0; i < n; ++i) cin >> a[i];

        string s;
        string err;
        bool ok = run_alice_sandbox(a, s, err, 3);
        if (!ok) {
#ifndef ONLINE_JUDGE
            // Print a concise failure tag and the message for debugging on local OJ.
            cout << "ALICE_FAIL " << err << "\n";
            cout.flush();
            continue;
#else
            throw runtime_error("ALICE_FAIL " + err);
#endif
        }

        std::cout << s << std::endl;

        vector<int> res;
        ok = run_bob_sandbox(n, s, res, err, 3);
        if (!ok) {
#ifndef ONLINE_JUDGE
            cout << "BOB_FAIL " << err << "\n";
            cout.flush();
            continue;
#else
            throw runtime_error("BOB_FAIL " + err);
#endif
        }

        // Output Bob's result as: cnt v0 v1 ...
        cout << res.size();
        for (int v : res) cout << ' ' << v;
        cout << '\n';
        cout.flush();
    }
    return 0;
}
