#include <bits/stdc++.h>
int main() {
    std::mt19937_64 rng(std::random_device{}());
    int TestCount = 100000;
    while (TestCount--) {
        int n = 200;
        std::vector<uint64_t> a(n);
        for (auto &x : a) x = rng();
        uint64_t res = 0, res2 = 0;
        for (auto x : a) for (auto y : a) res ^= x & y;
        for (auto x : a) res2 ^= x;
        if(res != res2) {
            puts("PoC failed;");
            return 2;
        }
    }
}