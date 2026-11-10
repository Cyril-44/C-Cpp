#include <bits/stdc++.h>
constexpr int N = 1000005, MOD = (int)1e9 + 7;
struct Mint {
    Mint& operator+=(const Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(const Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(const Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint x, const Mint y) { return x += y; }
    friend Mint operator-(Mint x, const Mint y) { return x -= y; }
    friend Mint operator*(Mint x, const Mint y) { return x *= y; }
    Mint(int v=0) : val(v) {}
private: int val;
} f[N];
template<class T> inline T qpow(T b, int n) {
    T res = 1;
    for (; n; n >>= 1, b *= b)
        if (n & 1) res *= b;
    return res;
}
int n, m;
inline Mint sum(int l, int r) {
    int lb = l / n, rb = r / n;
    l %= n, r %= n;
    if (lb == rb) return f[r] - (l?f[l-1]:0);
    Mint res = f[n-1] - (l?f[l-1]:0) + f[r];
    if (lb != rb) res += (rb - lb - 1) * f[n-1];
    return res;
}
inline Mint calc(int k, int m) {
    if (m <= k) return sum(k-m, k);
    return sum(0, k) + sum(0, m-k) - f[0];
}
int main() {
    freopen("offset.in", "r", stdin);
    freopen("offset.out", "w", stdout);
    int T, K;
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d", &K, &m);
        if (m == 0) {
            Mint ans = 1;
            while (K--) {
                int p, s; scanf("%d%d", &p, &s);
                ans *= qpow(p, s-1) * (p-1);
            }
            printf("%d\n", ans);
        } else {
            for (n = 1; K--; ) {
                int p, s; scanf("%d%d", &p, &s);
                n *= qpow(p, s);
            }
            f[0] = n;
            for (int i = 1; i < n; i++)
                f[i] = f[i-1] + std::__gcd(i, n);
            Mint ans = 0;
            for (int k = 1; k <= n; k++)
                if (std::__gcd(n, k) == 1)
                    ans += calc(k, m);
            printf("%d\n", ans);
        }
    }
}