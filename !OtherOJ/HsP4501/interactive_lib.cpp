// interactor.cpp
#include "testlib.h"
#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sched.h>

using namespace std;
using u64 = unsigned long long;

int get_fd() {
    char *fds = getenv("INTERACTOR_SHARED_MEMORY_FD");
    return atoi(fds);
}

// 与选手端必须一致的生成规则（与旧 interactor 的 rnd.next(0u,0xffffffffu) 等价）
static inline uint32_t next32(std::mt19937 &rng) {
    return std::uniform_int_distribution<uint32_t>(0u, 0xffffffffu)(rng);
}

int main(int argc, char *argv[]) {
    // 共享内存只需放 N, MOD, seed,
    size_t need = 5 * sizeof(u64);

    int fd = get_fd();
    void *base = mmap(NULL, need, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    assert(base != MAP_FAILED);

    registerInteraction(argc, argv);
    int N = inf.readInt(1, 10, "N");
    u64 seed = inf.readUnsignedLong();
    unsigned MOD = inf.readUnsignedLong();

    volatile u64 *shm = (volatile u64*)base;

    // 写入 N, MOD, seed，通知选手开始（N 非 0）
    shm[1] = (u64)MOD;
    shm[2] = seed;
    shm[0] = (u64)N;

    assert(N != 0);
    
    fputs("Start Interacting.(from int)\n", stderr); fflush(stderr);
    // 等待选手完成（选手会把 shm[0] 置 0）
    while (shm[0] != 0);
    fputs("Finish Interacting.(from int)\n", stderr); fflush(stderr);
    if (shm[3]) quitf(_ok, "All %d hashes match (N=%d)", N, N);
    else quitf(_wa, "Wrong answer on position %d", (int)shm[4]);
    return 0;
}
