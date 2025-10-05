#include <cstdio>
#include <set>
constexpr int N = 20, MOD = 998244353;
int a[N];
long long s[1<<N];
int f[1<<N], g[1<<N];
// f[i] -> 状态i中 最大前缀和为s[i]的方案数
// g[i] -> 状态i中 最大前缀和全部小于0（qzh[1~k]<0）的方案数
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) scanf("%d", &a[i]);
    for (int i = 0; i < n; i++) s[1<<i] = a[i], f[1<<i] = 1;
    for (int i = 0; i < (1<<n); i++)
        if (i & i-1) s[i] = s[i & i-1] + s[i & -i];
    g[0] = 1;
    for (int i = 0; i < (1 << n); ++i)
        for (int j = 0; j < n; j++)
            if (!(i & 1 << j)) {
                if (s[i] >= 0) (f[i | 1 << j] += f[i]) %= MOD;
                if (s[i | 1 << j] < 0) (g[i | 1 << j] += g[i]) %= MOD;
            }
    for (int i = 1; i <= n; i++) {
        int n;
        scanf("%d", &n);
    }
    const int U = (1 << n) - 1;
    int ans = 0;
    for (int i = 0; i < (1 << n); i++) {
        (ans += s[i] % MOD * f[i] % MOD * g[U ^ i] % MOD) %= MOD;
        if (ans < 0) ans += MOD;
    }
    printf("%d\n", ans);
    return 0;
}