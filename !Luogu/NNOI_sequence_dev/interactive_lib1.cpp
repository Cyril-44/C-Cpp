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

std::string Alice(std::vector<int> a);

std::vector<int> Bob(int n, std::string s);

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
    clearenv();

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
   - parent serializes vector<int> a to child
   - child deserializes, calls Alice, writes resulting string back
   - parent returns string (no validation)
*/
static std::string AliceSandbox(std::vector<int> a) {
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

        // Read vector<int> from p_in[0]
        uint64_t len = 0;
        if (xread_full(p_in[0], &len, sizeof(len)) != (ssize_t)sizeof(len)) {
            // read error/EOF -> exit child
            _exit(111);
        }
        std::vector<int> vec;
        vec.resize(len);
        for (uint64_t i = 0; i < len; ++i) {
            int64_t v = 0;
            if (xread_full(p_in[0], &v, sizeof(v)) != (ssize_t)sizeof(v)) _exit(112);
            vec[i] = (int)v;
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

        // serialize vector<int> to child
        uint64_t len = a.size();
        if (xwrite(p_in[1], &len, sizeof(len)) < 0) {
            // write error
        } else {
            for (uint64_t i = 0; i < len; ++i) {
                int64_t v = a[i];
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
   - child reads, calls Bob(n, s), serializes vector<int> back
*/
static std::vector<int> BobSandbox(int n, std::string s) {
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
        std::vector<int> res;
        try {
            res = Bob(n_in, in_s);
        } catch (...) {
            _exit(124);
        }

        // serialize vector<int> back
        uint64_t len = res.size();
        if (xwrite(p_out[1], &len, sizeof(len)) < 0) _exit(125);
        for (uint64_t i = 0; i < len; ++i) {
            int64_t v = res[i];
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
        std::vector<int> res;
        res.resize(len);
        for (uint64_t i = 0; i < len; ++i) {
            int64_t v = 0;
            if (xread_full(p_out[0], &v, sizeof(v)) != (ssize_t)sizeof(v)) {
                res.resize(i);
                break;
            }
            res[i] = (int)v;
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
    int t, n;
    if (!(std::cin >> t)) return 0;
    while (t--) {
        std::cin >> n;
        std::vector<int> a(n);
        for (int i = 0; i < n; i++) std::cin >> a[i];

        std::string data = AliceSandbox(std::move(a));
        std::cout << data << std::endl;

        std::vector<int> ans = BobSandbox(n, std::move(data));
        std::cout << ans.size();
        for (int i : ans) std::cout << ' ' << i;
        std::cout << std::endl;
    }
    return 0;
}
