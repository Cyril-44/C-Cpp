#include <bits/stdc++.h>
constexpr int N = 505, MOD = 998244353;
#ifdef CLANGD
#define N 5
#endif
struct Mint {
    Mint& operator+=(const Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(const Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(const Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint x, const Mint y) { return x += y; }
    friend Mint operator-(Mint x, const Mint y) { return x -= y; }
    friend Mint operator*(Mint x, const Mint y) { return x *= y; }
    Mint operator-() const { return val ? Mint(MOD - val) : Mint(); }
    Mint pow(int n) const {
        Mint res = 1, b = *this;
        for (; n; n >>= 1, b *= b)
            if (n & 1) res *= b;
        return res;
    }
    Mint(int v=0) : val(v) {}
private: int val;
} f[N*N];

int main() {
    freopen("sale.in", "r", stdin);
    freopen("sale.out", "w", stdout);
    int n, k;
    scanf("%d%d", &n, &k);
    f[0] = 1;
    for (int i = 1; i <= n-2; i++)
        for (int j = k; j >= i; j--)
            f[j] += f[j-i];
    printf("%d\n%d\n", n * (n-1) / 2 + 1, f[k]);
}