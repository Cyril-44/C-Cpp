#include <stdio.h>
#include <string.h>
constexpr int MOD = 998244353;
inline void add(int &x, int y) { if ((x += y) >= MOD) x -= MOD; }
inline int qpow(int b, int n) {
    int res = 1;
    while (n) {
        if (n & 1) res = 1ull * res * b % MOD;
        b = 1ull * b * b % MOD, n >>= 1;
    }
    return res;
}
struct Pt1_4 {
    constexpr static int N = 500, M = 8;
    int f[2][1<<M][(1<<M+2)]{};
    int fac[(1<<M+2) + 1], ifac[(1<<M+2) + 1];
    inline void initC(int n) {
        fac[0] = 1;
        for (int i = 1; i <= n; i++)
            fac[i] = 1ull * fac[i-1] * i % MOD;
        ifac[n] = qpow(fac[n], MOD-2);
        for (int i = n-1; i >= 0; i--)
            ifac[i] = 1ull * ifac[i+1] * (i+1) % MOD;
    }
    inline int C(int n, int m) {
        return 1ull * fac[n] * ifac[m] % MOD * ifac[n - m] % MOD;
    }
    inline void operator()(int n, int m, int c, int v) {
        int mask = (1 << m) - 1;
        initC(c-1 ? v << m : v);
        for (int i = 1; i <= v; i++)
            f[1][i & mask][i] = 1;
        for (int i = 2; i <= n; i++, v *= c) {
            for (int s = 0; s <= mask; s++)
                memset(f[i&1][s], 0, sizeof(int) * (v * c + 1));
            for (int s = 0; s <= mask; s++)
                for (int j = 1; j <= v; j++)
                    if (f[i&1^1][s][j])
                        for (int k = 1; k <= j * c; k++)
                            add(f[i&1][(s + k) & mask][k], 1ll * f[i&1^1][s][j] * C(j*c, k) % MOD);
        }
        for (int s = 0; s <= mask; s++) {
            int res = 0;
            for (int j = 1; j <= v; j++)
                add(res, f[n&1][s][j]);
            printf("%d ", res);
        }
        putchar('\n');
    }
};
int main() {
    freopen("skyscraper.in", "r", stdin);
    freopen("skyscraper.out", "w", stdout);
    int n, m, c, v;
    scanf("%d%d%d%d", &n, &m, &c, &v);
    { Pt1_4 sol; sol(n, m, c, v); }
    return 0;
}
