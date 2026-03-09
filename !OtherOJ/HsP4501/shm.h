// shm.h
#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sched.h>

std::vector<unsigned> mod(const std::vector<unsigned long long>& A, unsigned MOD);
int main() {
    using namespace std;
    using u64 = unsigned long long;
    
    fprintf(stderr, "FD: %s\n", getenv("INTERACTOR_SHARED_MEMORY_FD"));
    volatile u64 *shm = (volatile u64*)mmap(NULL, 5 * sizeof(u64), PROT_READ | PROT_WRITE, MAP_SHARED, atoi(getenv("INTERACTOR_SHARED_MEMORY_FD")), 0);
    assert(shm != MAP_FAILED && shm != NULL);
    fprintf(stderr, "Shared Memory Map %llu\n", (unsigned long long)shm);
    
    fputs("Waiting For interactor to write data....(from shm)\n", stderr); fflush(stderr);
    // 等待 interactor 写入 N (非 0)
    u64 aaacnt=0;
    do sched_yield(), __sync_synchronize(), ((++aaacnt & 0x3ffff) == 0 && fprintf(stderr, "shit! time=%ld, %llu %llu %llu %llu %llu\n", clock(), shm[0], shm[1], shm[2], shm[3], shm[4])) ; while (shm[0] == 0);
    fputs("Data wrote.(from shm)\n", stderr); fflush(stderr);

    int N = (int)shm[0];
    unsigned MOD = (unsigned)shm[1];
    u64 seed = shm[2];

    // 重建 A 并计算哈希（与 interactor 保持完全一致）
    std::mt19937_64 rng(seed);
    std::vector<unsigned long long> A(N);
    std::vector<unsigned> expected(N);
    for (int i = 0; i < N; ++i) {
        uint32_t basev = std::uniform_int_distribution<uint32_t>(0, MOD-1)(rng);
        uint32_t r = std::uniform_int_distribution<uint32_t>()(rng);
        A[i] = (u64)basev + (u64)r * (u64)MOD;
        expected[i] = basev;
    }

    fputs("Testing....(from shm)\n", stderr); fflush(stderr);
    std::vector<unsigned> B(std::move(mod(A, MOD)));
    shm[3] = 1;
    for (int i = 0; i < N; ++i) if (B[i] != expected[i]) { shm[3] = 0; shm[4] = i; break; }

    fputs("Test Finish. Wrote back to memory.(from shm)\n", stderr); fflush(stderr);
    // 通知 interactor 完成
    __sync_synchronize(); shm[0] = 0;
    fputs("Interaction Complete.(from shm)\n", stderr); fflush(stderr);

    return 0;
}
