// shm.h
#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sched.h>

using namespace std;
using u64 = unsigned long long;

vector<unsigned> mod(const vector<unsigned long long>& A, unsigned MOD);

static inline uint32_t next32(std::mt19937 &rng) {
    return std::uniform_int_distribution<uint32_t>(0u, 0xffffffffu)(rng);
}

int main() {
    char *fds = getenv("INTERACTOR_SHARED_MEMORY_FD");
    int fd = atoi(fds);

    size_t need = 5 * sizeof(u64);
    void *base = mmap(NULL, need, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    assert(base != MAP_FAILED);

    volatile u64 *shm = (volatile u64*)base;
    
    fputs("Waiting For interactor to write data....(from shm)\n", stderr); fflush(stderr);
    // 等待 interactor 写入 N (非 0)
    for (; shm[0] == 0; );
    fputs("Data wrote.(from shm)\n", stderr); fflush(stderr);

    int N = (int)shm[0];
    unsigned MOD = (unsigned)shm[1];
    u64 seed = shm[2];

    // 重建 A 并计算哈希（与 interactor 保持完全一致）
    std::mt19937 rng(seed);
    std::vector<unsigned long long> A(N);
    std::vector<unsigned> expected(N);
    for (int i = 0; i < N; ++i) {
        uint32_t basev = next32(rng);
        uint32_t r = next32(rng);
        A[i] = (u64)basev + (u64)r * (u64)MOD;
        expected[i] = basev;
    }

    fputs("Testing....(from shm)\n", stderr); fflush(stderr);
    std::vector<unsigned> B(std::move(mod(A, MOD)));
    shm[3] = 1;
    for (u64 i = 0; i < N; ++i) {
        if (B[i] != expected[i]) {
            shm[3] = 0;
            shm[4] = i;
            break;
        }
    }

    fputs("Test Finish. Wrote back to memory.(from shm)\n", stderr); fflush(stderr);

    fputs("Interaction Complete.(from shm)\n", stderr); fflush(stderr);
    // 通知 interactor 完成
    shm[0] = 0;

    return 0;
}
