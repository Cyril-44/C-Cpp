#include <bits/stdc++.h>

std::vector<unsigned> mod(const std::vector<unsigned long long>& A, unsigned MOD);
int main() {
    using namespace std;
    using u64 = unsigned long long;
    
    int N; unsigned MOD; u64 seed;
    cin >> N >> seed >> MOD;
    fprintf(stderr, "%d, %u, %llu\n", N, MOD, seed);
    fflush(stderr);

    std::mt19937_64 rng(seed);
    std::vector<unsigned long long> A(N);
    std::vector<unsigned> expected(N);
    for (int i = 0; i < N; ++i) {
        uint32_t basev = std::uniform_int_distribution<uint32_t>(0, MOD-1)(rng);
        uint32_t r = std::uniform_int_distribution<uint32_t>()(rng);
        A[i] = (u64)basev + (u64)r * (u64)MOD;
        expected[i] = basev;
    }

    std::vector<unsigned> B(std::move(mod(A, MOD)));
    for (int i = 0; i < N; ++i) if (B[i] != expected[i]) { cout << "0 " << i << endl; fputs("Wrong Answer!", stderr); return 0; }
    cout << 1 << endl;
    fputs("Accepted!", stderr);
    return 0;
}
