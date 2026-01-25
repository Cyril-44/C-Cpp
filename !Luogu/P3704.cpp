#include <stdio.h>
constexpr int N = 1000005, MOD = (int)1e9 + 7;
using ULL = unsigned long long;
unsigned f[N];
inline unsigned qpow(ULL b, ULL n) {
    if (n >= MOD) n %= MOD - 1;
    ULL res = 1;
    while (n) {
        if (n & 1) res = res * b % MOD;
        b = b * b % MOD, n >>= 1;
    }
    return res;
}
int mu[N], fib[N], ifib[N], _init_ = [](const int n = 1e6) {
    mu[1] = 1;
    for (int i = 1; i <= (n >> 1); i++)
        for (int j = i << 1; j <= n; j += i)
            mu[j] -= mu[i];
    fib[1] = 1, ifib[1] = 1;
    for (int i = 2; i <= n; i++) {
        fib[i] = (fib[i-1] + fib[i-2]) % MOD;
        ifib[i] = qpow(fib[i], MOD - 2);
    }
    for (int i = 0; i <= n; i++) f[i] = 1;
    for (int i = 1; i <= n; i++)
        for (int j = i, k = 1; j <= n; j += i, k++)
            switch (mu[k]) {
                case 1:  f[j] = (ULL)f[j] * (ULL)fib[i] % MOD;  break;
                case -1: f[j] = (ULL)f[j] * (ULL)ifib[i] % MOD; break;
            }
    for (int i = 1; i <= n; i++) f[i] = (ULL)f[i-1] * (ULL)f[i] % MOD;
return 0; }();
inline int min(int x, int y) { return x < y ? x : y; }
/*
\begin{aligned}
&\prod_{i=1}^n\prod_{j=1}^m f_{\gcd(i,j)}
\\ =&\prod_{d=1}^n f_d \sum_{i=1}^n\sum_{j=1}^m[\gcd(i,j)=d]
\\ =&\prod_{d=1}^n {f_d}^{\displaystyle\sum_{i=1}^n\sum_{j=1}^m[d|i]\cdot[d|j]\cdot\sum_{k|\frac n d \,\land\, k|\frac m d} \mu(k) \cdot \lfloor \frac{n}{kd} \rfloor \cdot \lfloor \frac{m}{kd} \rfloor}
\\ =&\prod_{kd=1}^n (\prod_{d|kd} {f_d}^k)^{\displaystyle\lfloor \frac{n}{kd}\rfloor\lfloor \frac{m}{kd}\rfloor}
\end{aligned}
*/
int main() {
    int T, n, m;
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d", &n, &m);
        if (n > m) n ^= m ^= n ^= m;
        ULL ans = 1;
        for (int l = 1, r; l <= n; l = r + 1) {
            r = min(n / (n / l), m / (m / l));
            ans = ans * qpow((ULL)f[r] * (ULL)qpow(f[l-1], MOD - 2) % MOD, (ULL)(n / l) * (ULL)(m / l)) % MOD;
        }
        printf("%llu\n", ans);
    }
    return 0;
}