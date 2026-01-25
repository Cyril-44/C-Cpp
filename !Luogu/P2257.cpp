#include <stdio.h>
constexpr int N = (int)1e7 + 5;
int p[N >> 3], mu[N], f[N];
long long fpre[N];
bool np[N], _init_sieve_ = [](const int n = 1e7){
    mu[1] = 1;
    for (int i = 2; i <= n; i++) {
        if (!np[i]) p[++p[0]] = i, mu[i] = -1;
        for (int j = 1, tp; j <= p[0] && (tp = i * p[j]) <= n; j++) {
            np[tp] = true;
            if (i % p[j] == 0) break;
            mu[tp] = -mu[i];
        }
    }
    for (int j = 1; j <= p[0]; j++)
        for (int i = p[j], k = 1; i <= n; i += p[j], k++)
            f[i] += mu[k];
    for (int i = 1; i <= n; i++) fpre[i] = f[i] + fpre[i-1];
return true; }();
/*
\begin{aligned}
&\sum_{i=1}^n\sum_{j=1}^m [\gcd(i,j) \in P]
\\ =&\sum_{p \in P}\sum_{i=1}^n\sum_{j=1}^m [\gcd(i,j)=p]
\\ =&\sum_{p \in P}\sum_{i=1}^n\sum_{j=1}^m [p | i \,\land\, p | j]\cdot\sum_{d|\frac i p\,\land\,d|\frac j p} \mu(d)
\\ =&\sum_{p \in P}\sum_{d=1}^{\lfloor \frac n p \rfloor}\mu(d)\cdot\lfloor \frac{n}{p\cdot d} \rfloor\cdot\lfloor \frac{m}{p\cdot d} \rfloor
\\ =&\sum_{pd=1}^n\lfloor \frac{n}{pd} \rfloor\cdot\lfloor \frac{m}{pd} \rfloor\cdot \sum_{p \in P}\mu(\frac{pd}{p})
\end{aligned}
*/
inline int min(int x, int y) { return x < y ? x : y; }
int main() {
    int T, n, m;
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d", &n, &m);
        if (n > m) n ^= m ^= n ^= m;
        long long ans = 0;
        for (int l = 1, r; l <= n; l = r + 1) {
            r = min(n / (n / l), m / (m / l));
            ans += (fpre[r] - fpre[l-1]) * (n / l) * (m / l);
        }
        printf("%lld\n", ans);
    }
    return 0;
}