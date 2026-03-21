// interactor.cpp
#include "testlib.h"
#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sched.h>

using namespace std;
using u64 = unsigned long long;
int main(int argc, char *argv[]) {
    // 共享内存只需放 N, MOD, seed,
    // fprintf(stderr, "FD: %s\n", getenv("INTERACTOR_SHARED_MEMORY_FD"));
    volatile u64 *shm = (volatile u64*)mmap(NULL, 5 * sizeof(u64), PROT_READ | PROT_WRITE, MAP_SHARED, atoi(getenv("INTERACTOR_SHARED_MEMORY_FD")), 0);
    assert(shm != MAP_FAILED && shm != NULL);
    // fprintf(stderr, "Shared Memory Map %llu\n", (unsigned long long)shm);
    
    registerInteraction(argc, argv);
    int N = inf.readInt(1, 100000000, "N");
    u64 seed = inf.readUnsignedLong();
    unsigned MOD = inf.readUnsignedLong();
    // fprintf(stderr, "Read N=%d, seed=%llu, MOD=%u\n", N, seed, MOD);

    // 写入 N, MOD, seed，通知选手开始（N 非 0）
    shm[1] = (u64)MOD; shm[2] = seed; shm[0] = (u64)N;
    
    // fprintf(stderr, "Wrote shm[1]=%llu, shm[2]=%llu, shm[0]=%llu\nStart Interacting.(from int)\n",shm[1], shm[2], shm[0]); fflush(stderr);
    // 等待选手完成（选手会把 shm[0] 置 0）
    u64 aaacnt=0;
    while (shm[0] != 0) std::this_thread::sleep_for(100ms);
    // fputs("Finish Interacting.(from int)\n", stderr); fflush(stderr);
    if (shm[3]) quitf(_ok, "All Mods match (N=%d)", N);
    else quitf(_wa, "Wrong answer on position %d", (int)shm[4]);
    return 0;
}
