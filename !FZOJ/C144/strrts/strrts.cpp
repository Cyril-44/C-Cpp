#include <stdio.h>
#include <string.h>
#include <algorithm>
constexpr int N = 2000005, MOD = 998244353;
char s[N];
using ULL = unsigned long long;
inline void add(int &x, int y) { if ((x += y) >= MOD) x -= MOD; }
inline int qpow(int b, int n) {
    int res = 1;
    while (n) {
        if (n & 1) res = (ULL)res * b % MOD;
        b = (ULL)b * b % MOD, n >>= 1;
    }
    return res;
}
int fac[N], ifac[N], ___ = [](int n = 2000000){
    fac[0] = 1;
    for (int i = 1; i <= n; i++)
        fac[i] = (ULL)fac[i-1] * i % MOD;
    ifac[n] = qpow(fac[n], MOD-2);
    for (int i = n; i >= 1; i--)
        ifac[i-1] = (ULL)ifac[i] * i % MOD;
    return 0;
}();
inline int C(int n, int m) {
    return (ULL)fac[n] * ifac[m] % MOD * ifac[n - m] % MOD;
}
int h[128], odd[128], even[128];
int main() {
    freopen("strrts.in", "r", stdin);
    freopen("strrts.out", "w", stdout);
    int n;
    scanf("%d %s", &n, s+1);
    for (int i = 1; i <= n; i++)
        ++h[s[i]];
    ULL prod = 1;
    for (char ch = 'a'; ch <= 'z'; ch++) {
        for (int i = 0; i <= h[ch]; i += 2)
           add(even[ch], C(h[ch], i));
        prod = prod * even[ch] % MOD;
    }
    int ans = prod;
    for (char ch = 'a'; ch <= 'z'; ch++) {
        for (int i = 1; i <= h[ch]; i += 2)
            add(odd[ch], C(h[ch], i));
        add(ans, (ULL)prod * qpow(even[ch], MOD-2) % MOD * odd[ch] % MOD);
    }
    add(ans, MOD - 1);
    printf("%d\n", ans);
    return 0;
}
