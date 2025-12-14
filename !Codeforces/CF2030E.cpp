#include <stdio.h>
#include <string.h>
const int N = 200005, MOD = 998244353;
int a[N], f[N];
long long pow2[N];
int main() {
    pow2[0] = 1;
    for (int i = 1; i <= 200000; i++)
        pow2[i] = (pow2[i-1] << 1) % MOD;
    int t, n;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        memset(f, 0, n << 2);
        for (int i = 0; i < n; i++) {
            scanf("%d", &a[i]);
            ++f[a[i]];
        }
        long long ans = 0, mul = 1;
        int cnt = 0;
        for (int i = 0; i < n; i++) {
            if (f[i] == 0) break;
            mul = mul * (pow2[f[i]] - 1ll + MOD) % MOD;
            cnt += f[i];
            ans = (ans + (i+1ll) * mul * (pow2[n - cnt])) % MOD;
        }
        printf("%lld\n", ans);
    }
    return 0;
}