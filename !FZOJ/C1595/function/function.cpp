#include <cstdio>
constexpr int MOD = 998244353;
int main() {
    long long n;
    scanf("%lld", &n);
    if (n == 1) puts("1");
    else {
        int ans = 1, b = 0;
        long long idx = 1;
        for (; idx + (1ll << b) <= n; idx += 1ll << b++)
            ans = (ans + (b + 2ll) * ((1ll << b) % MOD)) % MOD;
        ans = (ans + (b + 2ll) * ((n - idx) % MOD)) % MOD;
        printf("%lld\n", ans);
    }
    return 0;
}