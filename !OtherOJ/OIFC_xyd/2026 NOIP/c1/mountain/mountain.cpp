#include <bits/stdc++.h>
constexpr int N = 5005, MOD = (int)1e9 + 7;
#ifdef CLANGD
#define N 55
#endif
struct Mint {
    Mint& operator+=(const Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(const Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(const Mint o) { val = 1ull * val * o.val % MOD; return *this;  }
    friend Mint operator+(Mint x, const Mint y) { return x += y; }
    friend Mint operator-(Mint x, const Mint y) { return x -= y; }
    friend Mint operator*(Mint x, const Mint y) { return x *= y; }
    Mint(int v=0) : val(v) {}
private: int val;
} fac[N], f[N][N], g[N][N];
char s[N];
int main() {
    freopen("mountain.in", "r", stdin);
    freopen("mountain.out", "w", stdout);
    scanf("%*d %s", s+1);
    int n = strlen(s+1);
    if (n == 1) {
        puts("1");
        return 0;
    }
    fac[0] = 1; for (int i = 1; i <= n; i++) fac[i] = fac[i-1] * i;
    f[0][0] = 1;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= i; j++)
            f[i][j] = s[i] == 'D'
                    ? f[i-1][j] * j + f[i-1][j+1] * (j+1)*j
                    : f[i-1][j-1] + f[i-1][j] * j;
    g[n+1][0] = 1;
    for (int i = n; i >= 1; i--)
        for (int j = 1; j <= n-i+1; j++)
            g[i][j] = s[i] == 'U'
                    ? g[i+1][j] * j + g[i+1][j+1] * (j+1)*j
                    : g[i+1][j-1] + g[i+1][j] * j;
    for (int i = 1; i <= n; i++) {
        Mint ans = 0;
        for (int j = 0; j < i; j++)
            ans += fac[j] * f[i-1][j] * (fac[j+1] * g[i+1][j+1] + fac[j]*2 * g[i+1][j] + (!j?0: fac[j-1] * g[i+1][j-1]));
        printf("%lld ", ans);
    }
    return 0;
}