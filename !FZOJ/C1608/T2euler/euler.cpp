#include <bits/stdc++.h>
namespace atcoder {
// @param m `1 <= m`
// @return x mod m
constexpr long long safe_mod(long long x, long long m) {
    x %= m;
    if (x < 0) x += m;
    return x;
}
// @param n `0 <= n`
// @param m `1 <= m`
// @return `(x ** n) % m`
constexpr long long pow_mod_constexpr(long long x, long long n, int m) {
    if (m == 1) return 0;
    unsigned int _m = (unsigned int)(m);
    unsigned long long r = 1;
    unsigned long long y = safe_mod(x, m);
    while (n) {
        if (n & 1) r = (r * y) % _m;
        y = (y * y) % _m;
        n >>= 1;
    }
    return r;
}
// Reference:
// M. Forisek and J. Jancina,
// Fast Primality Testing for Integers That Fit into a Machine Word
// @param n `0 <= n`
constexpr bool is_prime_constexpr(int n) {
    if (n <= 1) return false;
    if (n == 2 || n == 7 || n == 61) return true;
    if (n % 2 == 0) return false;
    long long d = n - 1;
    while (d % 2 == 0) d /= 2;
    constexpr long long bases[3] = {2, 7, 61};
    for (long long a : bases) {
        long long t = d;
        long long y = pow_mod_constexpr(a, t, n);
        while (t != n - 1 && y != 1 && y != n - 1) {
            y = y * y % n;
            t <<= 1;
        }
        if (y != n - 1 && t % 2 == 0) {
            return false;
        }
    }
    return true;
}
}
constexpr int MX = 1e9, RG = 1e4;
int64_t a[RG];
inline int phi(int x) {
    int ans = 1;
    for (int i = 2, i2 = i*i; i2 <= x; i2 += i++ << 1 | 1)
        if (x % i == 0) {
            ans *= (i-1), x /= i;
            while (x % i == 0)
                x /= i, ans *= i;
        }
    if (x > 2) ans *= x-1;
    return ans;
}
inline bool is_prime(int x) { return phi(x) == x-1; }
inline int64_t fphi(int x, int n) {
    while (n--) x = phi(x);
    return x;
}
int main() {
    int B, T;
    scanf("%d%d", &T, &B);
    int L = B + 2;
    for (; L <= MX && !atcoder::is_prime_constexpr(L); L++);
    if (L > MX) {
        for (int l, r; T--; ) {
            scanf("%d%d", &l, &r);
            printf("%lld\n", (r - l + 1ll) * (l + r) / 2);
        }
        return 0;
    }
    int R = std::min(L + 32, MX + 1);
    for (; R <= MX && !atcoder::is_prime_constexpr(R); R++);
    for (int i = L, n = 0; i < R; i++) {
        if (atcoder::is_prime_constexpr(i))
            n = i-1-B, a[i-L+1] = fphi(i-1, n-1);
        else a[i-L+1] = fphi(i, n);
        a[i-L+1] += a[i-L];
    }
    // [1, L):   f(x)=x
    // [L, R):   f(x)=a[x-L+1]
    // [R, +oo): f(x)=1
    auto f1 = [] (int l, int r) { return (r - l + 1ll) * (l + r) / 2; };
    auto f2 = [L](int l, int r) { return a[r-L+1] - a[l-L]; };
    auto f3 = [] (int l, int r) { return r - l + 1; };
    for (int l, r; T--; ) {
        scanf("%d%d", &l, &r);
        int64_t ans = 0;
        if (l < L) ans += f1(l, std::min(L-1, r));
        if (l < R && r >= L) ans += f2(std::max(l, L), std::min(R-1, r));
        if (r >= R) ans += f3(std::max(R, l), r);
        printf("%lld\n", ans);
    }
}