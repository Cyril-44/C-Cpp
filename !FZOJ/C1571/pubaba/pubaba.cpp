#include <cstdio>
#include <algorithm>
#include <cstdint>
struct FastI {
    char buf[1 << 20], *p1, *p2;
    FastI() : p1(), p2() {}
    [[gnu::always_inline]] inline char gc() {
        if (p1 == p2) p2 = (p1=buf) + fread(buf, 1, sizeof buf, stdin);
        return *p1++;
    }
    template<typename T>
    inline void operator()(T &x) {
        char ch = gc();
        while (ch < '0' || ch > '9') ch = gc();
        for (x = 0; ch >= '0' && ch <= '9'; ch = gc())
            x = (x << 3) + (x << 1) + (ch ^ '0');
    }
} in;
constexpr int N = 5000005;
int64_t lsum[N], rsum[N];
int lcnt[N], rcnt[N];
int main() {
    int n, m;
    int64_t S;
    in(n), in(m), in(S);
    for (int i = 1, l, r; i <= m; i++) {
        in(l), in(r);
        lsum[l] += l, rsum[r] += r;
        ++lcnt[l], ++rcnt[r];
    }
    for (int i = 1; i <= n; i++) rsum[i] += rsum[i-1], rcnt[i] += rcnt[i-1];
    for (int i = n; i >= 1; i--) lsum[i] += lsum[i+1], lcnt[i] += lcnt[i+1];
    if (lsum[1] > S || rsum[n] < S) return puts("-1"), 0;
    uint64_t ans = 0;
    for (int i = n; i >= 1; i--) {
        /* int64_t sum1 = 0, sum2 = 0;
        for (int j = 1; j <= m; j++) {
            sum1 += std::max(0, a[j].L - (n-i+1));
            sum2 += std::max(0, (n-i+1) - a[j].R);
        } */
        uint64_t sum1 = lsum[i] - 1l*i * lcnt[i],
                 sum2 =-rsum[i] + 1l*i * rcnt[i];
        ans ^= std::min((S - sum1) / i, m - (sum2 + i - 1) / i) * (n - i + 1);
        // printf("%ld %ld\n", (S-sum1)/(n-i+1), m - (sum2 + n-i+1 - 1)/(n-i+1), " \n"[i==n]);
    }
    printf("%ld\n", ans);
    return 0;
}
