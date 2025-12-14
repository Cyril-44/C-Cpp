#include <cstdio>
constexpr int N = 405, MOD = 1e9 + 7;
inline int qpow(int b, int n) {
    int res = 1;
    while (n) {
        if (n & 1) res = 1ll * res * b % MOD;
        b = 1ll * b * b % MOD;
        n >>= 1;
    }
    return res;
}
inline void fix(int &x) {
    if (x >= MOD) x -= MOD;
    else if (x < 0) x += MOD;
}
int main() {
    int n, m, c;
    scanf("%d%d%d", &n, &m, &c);
    int ans = 0, cci = 1, cnj;
    for (int i = 1; i <= c; i++) {
        int fi = 0; cnj = 1;
        for (int j = 0; j <= n; j++) {
            if (j) cnj = cnj * (n-j+1ll) % MOD * qpow(j, MOD - 2) % MOD;
            fix(fi += (j & 1 ? -1 : 1) * qpow(qpow(i + 1, (n - j)) - 1, m) * 1ll * cnj % MOD);
        }
        cci = cci * (c-i+1ll) % MOD * qpow(i, MOD - 2) % MOD;
        fix(ans += (c - i & 1 ? -1 : 1) * 1ll * fi * cci % MOD);
    }
    printf("%d\n", ans);
    return 0;
}