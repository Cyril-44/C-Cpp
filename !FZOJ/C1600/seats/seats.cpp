#include <bits/stdc++.h>
constexpr int N = 1000004;
uint64_t a[N];
int main() {
    std::mt19937 rng(std::random_device{}());
    int n;
    scanf("%d", &n);
    std::uniform_int_distribution<int> posGen(1, n);
    uint64_t mnval = 0, m = 2;
    for (int i = 1; i <= n; i++)
        scanf("%llu", &a[i]), mnval += a[i] & 1;
    auto work = [&](uint64_t x) {
        if (x <= 2) return;
        uint64_t cnt = 0;
        for (int i = 1; i <= n; i++) {
            cnt += a[i] % x;
            if (cnt > mnval) break;
        }
        if (cnt < mnval) mnval = cnt, m = x;
        else if (cnt == mnval && x < m) m = x;
    };
    for (int _ = 20; _--; ) {
        int pos = posGen(rng);
        for (unsigned i = 1, lm = sqrt(a[pos]); i <= lm; i++)
            if (a[pos] % i == 0) work(i), work(a[pos] / i);
    }
    printf("%llu\n", m);
    return 0;
}