#include <stdio.h>
#include <string.h>
constexpr int N = 200005, MOD = (int)1e9 + 7;
inline void add(int &x, int y) { if ((x += y) >= MOD) x -= MOD; }
inline int qpow(int b, int n) {
    int res = 1;
    while (n) {
        if (n & 1) res = 1ull * res * b % MOD;
        b = 1ull * b * b % MOD, n >>= 1;
    }
    return res;
}
int fac[N], ifac[N];
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
struct Pt1_4 {
    constexpr static int N = 505;
    int f[2][N][N];
    void operator()(int n, int m) {
        initC(m);
        for (int i = 1; i <= m; i++)
            f[1][i][i] = 1;
        for (int i = 2; i <= n; i++) {
            memset(f[i&1], 0, sizeof(f) / 2);
            for (int s = 1; s < m; s++)
                for (int j = 1; j <= s; j++)
                    if (f[i&1^1][s][j])
                        for (int k = 1; k + s <= m; k++)
                            add(f[i&1][s+k][k], 1ull * f[i&1^1][s][j] * C(j + k - 1, k) % MOD);
        }
        int res = 0;
        for (int s = 1; s <= m; s++)
            for (int j = 1; j <= s; j++)
                add(res, f[n&1][s][j]);
        printf("%d\n", res);
    }
};
struct Pt7 {
    void operator()(int m) {
        int res = 0;
        for (int s = 1; s < m; s++) { // s = b_1 + b_2 - 1
            add(res, (qpow(2, s) - 1 + MOD) % MOD);
        }
        printf("%d\n", res);
    }
};
int main() {
    freopen("recall.in", "r", stdin);
    freopen("recall.out", "w", stdout);
    int n, m;
    scanf("%d%d", &n, &m);
    if (n == 2) { Pt7 sol; sol(m); }
    else { Pt1_4 sol; sol(n, m); }
    return 0;
}