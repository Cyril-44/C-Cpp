#include <cstdio>
#include <algorithm>
#include <numeric>
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
constexpr int N = 50005;
std::pair<int,int> a[N];
#define L first
#define R second
int64_t lsum[N], rsum[N];
int main() {
    int n, m;
    int64_t S;
    in(n), in(m), in(S);
    for (int i = 1; i <= m; i++) {
        in(a[i].L), in(a[i].R);
        if (a[i].L > n) return puts("-1"), 0;
        lsum[i] = lsum[i-1] + a[i].L;
        rsum[i] = rsum[i-1] + a[i].R;
    }
    std::sort(a+1, a+1 + m);
    if (! ( std::accumulate(a+1, a+1+m, 0ll, [](const int64_t acc, const std::pair<int,int> p){ return acc + p.L; }) <= S && 
            std::accumulate(a+1, a+1+m, 0ll, [](const int64_t acc, const std::pair<int,int> p){ return acc + p.R; }) >= S ))
        return puts("-1"), 0;
    int64_t ans = 0;
    for (int i = 1; i <= n; i++) {
        int64_t sum = 0;
        for (int j = 1; j <= m; j++)
            if (a[j].L > (n - i + 1)) sum += a[j].L - (n - i + 1);
        int tmp = n - i + 1, mx = m;
        for (int j = 1; j <= m; j++) {
            if (tmp < a[j].L) break;
            --mx, tmp -= a[j].L;
        }
        ans ^= std::min((int64_t)mx, (S - sum) / (n - i + 1)) * i;
        printf("%ld%c", std::min((int64_t)mx, (S - sum) / (n - i + 1)), " \n"[i==n]);
    }
    printf("%ld\n", ans);
    return 0;
}
