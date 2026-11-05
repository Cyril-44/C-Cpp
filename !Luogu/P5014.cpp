#include <bits/stdc++.h>
constexpr int N = 2000005, MOD = 998244353;
struct Mint {
    Mint& operator+=(const Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(const Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(const Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint x, const Mint y) { return x += y; }
    friend Mint operator-(Mint x, const Mint y) { return x -= y; }
    friend Mint operator*(Mint x, const Mint y) { return x *= y; }
    Mint(int v=0) : val(v % MOD) {}
    Mint pow(int n) {
        Mint res = 1, b = *this;
        for (; n; n >>= 1, b *= b)
            if (n & 1) res *= b;
        return res;
    }
    template<class T> explicit operator T() const { return static_cast<T>(val); }
private: int val;
} fac[N], ifac[N];
inline Mint C(int n, int m) { return fac[n] * ifac[m] * ifac[n-m]; }
inline Mint Catalan(int n, int m) { return m == 0 ? 1 : C(n+m, m) - C(n+m, m-1); }
int main() {
    int T; int64_t u;
    fac[0] = 1;
    constexpr int _n = 2e6;
    for (int i = 1; i <= _n; i++) fac[i] = fac[i-1] * i;
    ifac[_n] = fac[_n].pow(MOD-2);
    for (int i = _n; i >= 1; i--) ifac[i-1] = ifac[i] * i;
    scanf("%d", &T);
    while (T--) {
        scanf("%lld", &u);
        int n = 1;
        while (n * (n+1ll) / 2 < u) ++n;
        int m = u - n * (n-1ll) / 2;
        --n, --m;
        Mint ans = 0;
        for (int i = 0; i <= m; i++)
            ans += Catalan(n-i, m-i) * C(n+m-i, i);
        printf("%d\n", ans);
    }
}