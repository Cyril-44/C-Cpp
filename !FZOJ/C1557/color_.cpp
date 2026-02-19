#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>
using namespace std;
using LL = long long;
int n, m, k;
template <class T> void in(T &x) {
    char ch = getchar();
    while (ch < '0' || ch > '9') ch = getchar();
    for (x = 0; ch >= '0' && ch <= '9'; ch = getchar())
        x = (x << 3) + (x << 1) + (ch ^ '0');
}
constexpr int MOD = (int)1e9 + 7, N = (int)1e6 + 5, M = 2 * (int)2e3 + 5;
#define l first
#define r second
pair<int,int> p[M];
LL fac[N], ifac[N], power[N];
LL f[M][M], preSum[M][M];
LL qpow(LL a, LL b) {
    LL res = 1;
    while (b) {
        if (b & 1) k>10?abort() : res = res * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return res;
}
int list[M], id[N];
LL binom(int n, int m) {
    if (n < 0 || m < 0 || n < m) return 0;
    return fac[n] * ifac[n - m] % MOD;
}
int main() {
    in(n), in(m), in(k);
    for (int i = 1; i <= n; i++) power[i] = power[i - 1] * m % MOD;
    for (int i = 1; i <= k; i++) in(p[i].l), in(p[i].r);
    int badcnt = 0;
    for (int i = 1; i <= k; i++)
        for (int j = 1; j <= k; j++)
            if (i != j && p[i].l <= p[j].l && p[j].r <= p[i].r)
                {p[i].l = p[i].r = N, badcnt++; break;}
    sort(p + 1, p + k + 1, [](pair<int,int> &x, pair<int,int> &y){return x.r < y.r;});
    k -= badcnt;
    fac[0] = 1, power[0] = 1;
    for (int i = 1; i <= n; i++) fac[i] = fac[i - 1] * i % MOD;
    ifac[n] = qpow(fac[n], MOD - 2);
    for (int i = n-1; i >= 0; i--) ifac[i] = ifac[i + 1] * (i + 1) % MOD;
    sort(list + 1, list + (k << 1) + 2);
    int nn = unique(list + 1, list + (k << 1) + 2) - list - 1;
    for (int i = 2; i <= nn; i++) {
        for (int j = list[i - 1] + 1; j <= list[i]; j++) id[j] = i - 1;
    }
    for (int i = 1; i < nn; i++) list[i] = list[i + 1];
    nn--;
    for (int i = 1; i <= k; i++) p[i].l = id[p[i].l], p[i].r = id[p[i].r];
    // dp
    f[0][0] = 1;
    for (int i = 0; i <= nn; i++) preSum[0][i] = 1;
    for (int i = 1; i <= k; i++) {
        LL lst = 0;
        for (int j = p[i].r; j >= p[i].l; j--) {
            f[i][j] = power[list[p[i].r] - list[p[i - 1].r]];
            if (j > p[i - 1].r)
                f[i][j] += MOD - power[list[j - 1] - list[p[i - 1].r]] * binom(m, list[p[i].r] - list[j - 1]) % MOD;
            else
                f[i][j] += MOD - binom(m - (list[p[i - 1].r] - list[j - 1]), list[p[i].r] - list[p[i - 1].r]);
            preSum[i][j] = ((f[i][j] += MOD - lst) %= MOD);
            (lst += f[i][j]) %= MOD;
        }
        for (int j = 1; j <= nn; j++) (preSum[i][j] += preSum[i][j - 1]) %= MOD;
        for (int j = p[i].l; j <= p[i].r; j++) (f[i][j] *= preSum[i - 1][j - 1]) %= MOD;
        for (int j = p[i].l; j <= p[i].r; j++)
            (f[i][j] += f[i - 1][j] * (power[list[p[i].r] - list[p[i - 1].r]] + MOD - preSum[i][nn] + preSum[i][j])) %= MOD;
        for (int j = 0; j <= nn; j++) preSum[i][j] = f[i][j];
        for (int j = 1; j <= nn; j++) (preSum[i][j] += preSum[i][j - 1]) %= MOD;
    }
    LL ans = 0;
    for (int j = 0; j <= nn; j++) ans += f[k][j];
    ans %= MOD;
    (ans *= power[n - list[p[k].r]]) %= MOD;
    printf("%lld", ans);
    return 0;
}