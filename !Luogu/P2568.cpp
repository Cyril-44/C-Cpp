#include <stdio.h>
constexpr int N = 10000005;
using LL = long long;
int p[N >> 3], mu[N], premu[N];
bool np[N], _init_p = [](const int n = 1e7) {
    for (int i = 2; i <= n; i++) {
        if (!np[i]) p[++p[0]] = i, mu[i] = -1;
        for (int j = 1, tp; j <= p[0] && (tp = i * p[j]) <= n; j++) {
            np[tp] = true;
            if (i % p[j] == 0)
                { mu[tp] = 0; break; }
            mu[tp] = -mu[i];
        }
    }
    mu[1] = 1;
    for (int i = 1; i <= n; i++)
        premu[i] = mu[i] + premu[i-1];
    return false;
}();
inline LL calc(int n) {
    LL res = 0;
    for (int l = 1, r; l <= n; l = r + 1) {
        r = n / (n / l);
        res += (LL)(n / l) * (n / l) * (premu[r] - premu[l-1]);
    }
    return res;
}
inline void write(__int128 tp) {
    static int sta[40];
    sta[sta[0] = 1] = tp % 10;
    while (tp /= 10) sta[++sta[0]] = tp % 10;
    while (sta[0]) putchar(sta[sta[0]--] | '0');
}
int main() {
    int n;
    scanf("%d", &n);
    __int128 ans = 0;
    for (int i = 1; i <= p[0] && p[i] <= n; i++)
        ans += calc(n / p[i]);
    write(ans), putchar('\n');
    return 0;
}