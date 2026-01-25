#include <stdio.h>
constexpr int N = 50005;
/*
\begin{aligned}
f(n,m,p)=&\sum_{i=1}^n\sum_{j=1}^m [\gcd(i,j)=p]
\\ =&\sum_{d=1}^{\lfloor \frac n p \rfloor}\mu(d)\cdot\lfloor \frac{n}{p\cdot d} \rfloor\cdot\lfloor \frac{m}{p\cdot d} \rfloor
\\ \sum_{i=a}^b\sum_{j=c}^d [\gcd(i,j)=p] =&f(b,d,p)-f(a-1,d,p)-f(b,c-1,p)+f(a-1,c-1,p)
\end{aligned}
*/
int mu[N], musum[N], _init_ = [](const int n = 5e4){
    mu[1] = 1;
    for (int i = 1; i <= (n >> 1); i++)
        for (int j = i << 1; j <= n; j += i)
            mu[j] -= mu[i];
    for (int i = 1; i <= n; i++) musum[i] = musum[i-1] + mu[i];
return 0; }();
inline int min(int x, int y) { return x < y ? x : y; }
inline long long f(int n, int m, int p) {
    if (n > m) n ^= m ^= n ^= m;
    n /= p, m /= p;
    long long res = 0;
    for (int l = 1, r; l <= n; l = r + 1) {
        r = min(n / (n / l), m / (m / l));
        res += 1ll * (musum[r] - musum[l-1]) * (n / l) * (m / l);
    }
    return res;
}
int main() {
    int T, a, b, c, d, p;
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d%d%d%d", &a, &b, &c, &d, &p); --a, --c;
        printf("%lld\n", f(b,d,p) - f(a,d,p) - f(b,c,p) + f(a,c,p));
    }
    return 0;
}