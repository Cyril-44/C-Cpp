#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int mod = 1000000007;
int tid, n;
ll fac[5005], f[5005][5005], g[5005][5005], ans;
char s[5005];
int main() {
    freopen("mountain.in", "r", stdin);
    freopen("mountain.out", "w", stdout);
    scanf("%d%s", &tid, s + 1);
    n = strlen(s + 1);
    fac[0] = 1;
    for (int i = 1; i <= n; i++) fac[i] = fac[i - 1] * i % mod;
    f[0][0] = 1;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= i; j++) {
            if (s[i] == 'D')
                f[i][j] = (f[i - 1][j] * j + f[i - 1][j + 1] * (j + 1) * j) % mod;
            else
                f[i][j] = (f[i - 1][j - 1] + f[i - 1][j] * j) % mod;
        }
    }
    g[n + 1][0] = 1;
    for (int i = n; i >= 1; i--) {
        for (int j = 1; j <= n - i + 1; j++) {
            if (s[i] == 'U')
                g[i][j] = (g[i + 1][j] * j + g[i + 1][j + 1] * (j + 1) * j) % mod;
            else
                g[i][j] = (g[i + 1][j - 1] + g[i + 1][j] * j) % mod;
        }
    }
    for (int i = 1; i <= n; i++) {
        ans = 0;
        for (int j = 0; j < i; j++) {
            if (j) ans = (ans + fac[j] * fac[j - 1] % mod * f[i - 1][j] % mod * g[i + 1][j - 1]) % mod;
            ans = (ans + fac[j] * fac[j] * 2 % mod * f[i - 1][j] % mod * g[i + 1][j]) % mod;
            ans = (ans + fac[j] * fac[j + 1] % mod * f[i - 1][j] % mod * g[i + 1][j + 1]) % mod;
        }
        printf("%lld ", ans);
    }
    printf("\n");
    return 0;
}