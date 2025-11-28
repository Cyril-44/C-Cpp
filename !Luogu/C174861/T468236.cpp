#include <stdio.h>
typedef long long LL;
const LL MOD = 998244353;
inline LL qpow(LL b, LL n) {
    LL res = 1;
    while (n) {
        if (n & 1) res = res * b % MOD;
        b = b * b % MOD;
        n >>= 1;
    }
    return res;
}
int main() {
    int n;
    scanf("%d", &n);
    long long ans = 1;
    for (int i = 1; i <= n; i++) {
        ans = ans * qpow(qpow(i, i), ((n - i + 2ll) * (n - i + 1ll) / 2)) % MOD;
    }
    printf("%lld\n", ans);
    return 0;
}
/*
1
1 1 2
1 1 2 1 2 3
*/