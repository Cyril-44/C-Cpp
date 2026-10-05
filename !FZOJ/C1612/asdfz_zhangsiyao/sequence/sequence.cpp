#include <bits/stdc++.h>
#ifdef CLANGD
constexpr int N = 25;
#else
constexpr int N = 200005;
#endif
struct Mint {
    constexpr static int MOD = 998244353;
    Mint& operator+=(Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint x, Mint y) { return x += y; }
    friend Mint operator-(Mint x, Mint y) { return x -= y; }
    friend Mint operator*(Mint x, Mint y) { return x *= y; }
    Mint (int v=0) : val(v) {}
    template<typename T> explicit operator T() const { return static_cast<T>(val); }
private: int val;
} f[20][N], g[20], h[N];
int main() {
    freopen("sequence.in", "r", stdin);
    freopen("sequence.out", "w", stdout);
    int n, m;
    scanf("%d%d", &n, &m);
    for (int j = 1; j <= m; j++) f[1][j] = 1;
    for (int i = 2; i < 20; i++)
        for (int j = 1; j <= m; j++)
            for (int k = j + j; k <= m; k += j)
                f[i][k] -= f[i-1][j];
    for (int i = 0; i < 20; i++)
        for (int j = 1; j <= m; j++)
            g[i] += f[i][j];
    h[0] = 1;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= i && j < 20; j++)
            h[i] += h[i-j] * g[j];
    printf("%d\n", h[n]);
}