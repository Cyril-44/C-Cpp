#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <algorithm>
using LL = unsigned long long;
using i128 = unsigned __int128;
constexpr int N = 1000005;
constexpr LL MOD = (LL)1e18 + 31, G = 42, A0 = 1145141919810ULL;
LL qpow(LL b, LL n) {
    LL res = 1;
    while (n) {
        if (n & 1) res = (i128)res * b % MOD;
        b = (i128)b * b % MOD;
        n >>= 1;
    }
    return res;
}
int main() {
    freopen("log.in", "r", stdin);
    freopen("log.out", "w", stdout);
    int n;
    scanf("%d", &n);
    LL ans = A0;
    for (int i = 1000000 - n; i--; ) {
        ans = qpow(G, ans);
    }
    printf("%llu\n", ans);
    return 0;
}
