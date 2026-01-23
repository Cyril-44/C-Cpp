#include <stdio.h>
constexpr int N = 50005;
namespace FastI {
    char buf[1 << 20], *p1{}, *p2{};
    [[gnu::always_inline]] inline char gc() {
        if (__builtin_expect(p1 == p2, 0)) p2 = (p1 = buf) + fread(buf, 1, sizeof buf, stdin);
        return *p1++; 
    }
    inline void in(int &x) {
        char ch = gc();
        while (ch < '0' || ch > '9') ch = gc();
        for (x = 0; ch >= '0' && ch <= '9'; ch = gc())
            x = (x << 3) + (x << 1) + (ch & 0xf);
    }
} using FastI::in;
/*
d(ij) = \sum_{d|i\land d|j}\mu(d) \cdot \tau_{\text{sum}}(\lfloor\frac n d \rfloor) \cdot \tau_{\text{sum}}(\lfloor\frac m d \rfloor) \\
\sum_{i=1}^n\sum_{j=1}^m = \sum_{d=1}^{\min(n,m)} \mu(d) \cdot \tau_{\text{sum}}(\lfloor \frac n d \rfloor) \cdot \tau_{\text{sum}}(\lfloor \frac m d \rfloor)
*/
long long tausum[N];
int mu[N], musum[N], tau[N], _init_ = [](int n = 5e4) {
    mu[1] = 1;
    for (int i = 1; i <= (n >> 1); i++)
        for (int j = i << 1; j <= n; j += i)
            mu[j] -= mu[i];
    for (int i = 1; i <= n; i++)
        for (int j = i; j <= n; j += i)
            ++tau[j];
    for (int i = 1; i <= n; i++)
        tausum[i] = tausum[i-1] + tau[i];
    for (int i = 1; i <= n; i++)
        musum[i] += musum[i-1] + mu[i];
    return 0;
}();
inline int min(int x, int y) { return x < y ? x : y; }
int main() {
    int T, n, m;
    in(T);
    while (T--) {
        in(n), in(m);
        if (n > m) n^=m^=n^=m;
        long long ans = 0;
        for (int l = 1, r, x, y; l <= n; l = r + 1) {
            r = min(n / (x = n / l), m / (y = m / l));
            ans += (musum[r] - musum[l-1]) * tausum[x] * tausum[y];
        }
        printf("%lld\n", ans);
    }
    return 0;
}