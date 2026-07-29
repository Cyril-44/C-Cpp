#include <bits/stdc++.h>
constexpr int N = 5005, MOD = 998244353;
int64_t a[N], d[N], r[N];
struct Mint {
    Mint& operator+=(Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint a, Mint b) { return a += b; }
    friend Mint operator-(Mint a, Mint b) { return a -= b; }
    friend Mint operator*(Mint a, Mint b) { return a *= b; }
    Mint(int v=0) : val(v) {}
    Mint(int64_t v) : val(v % MOD) {}
    template<class T> explicit operator T() const { return static_cast<T>(val); }
private: int val;
};
int main() {
    int Tid, n;
    int64_t L;
    scanf("%d%d%lld", &Tid, &n, &L);
    if (L < n) { puts("0"); return 0; }
    for (int i = 1; i <= n; i++)
        scanf("%lld", &a[i]);
    std::sort(a+1, a+1+n, [L](int64_t x, int64_t y){ return x%L < y%L; });
    // 拆成 (< d[i]*L) + (>= d[i]*L)
    Mint f[N]{1};
    for (int i = 1; i <= n; i++) {
        Mint g[N]{};
        d[i] = a[i] / L, r[i] = a[i] % L;
        for (int j = 0; j < i; j++) {
            if (j < r[i]) g[j+1] += f[j] * (r[i] - j);
            g[j] += f[j] * d[i];
        }
        memcpy(f, g, sizeof g);
    }
    Mint ans = 0, prod = 1;
    for (int i = n; i >= 0; i--) {
        ans += f[i] * prod;
        prod *= (L - i + 1);
    }
    printf("%d\n", ans);
    return 0;
}