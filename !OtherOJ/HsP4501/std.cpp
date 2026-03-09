// solution.cpp
#include "shm.h"

std::vector<unsigned> mod(const std::vector<unsigned long long>& A, unsigned MOD) {
    std::vector<unsigned> res(A.size());
    uint64_t m = MOD;
    uint64_t mu = ~0ULL / m;  // floor(2^64 / m)
    // raise(114); abort();
    for (size_t i = 0; i < A.size(); i++) {
        uint64_t x = A[i];
        uint64_t q = (__uint128_t(x) * mu) >> 64;
        uint64_t r = x - q * m;
        if (r >= m) r -= m;
        res[i] = (unsigned)r;
    }
    return res;
}
