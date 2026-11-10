#include <bits/stdc++.h>
constexpr int N = 5005, MOD = (int)1e9 + 7;
int b[N];
struct Mint {
    Mint& operator+=(const Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(const Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(const Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint x, const Mint y) { return x += y; }
    friend Mint operator-(Mint x, const Mint y) { return x -= y; }
    friend Mint operator*(Mint x, const Mint y) { return x *= y; }
    Mint(int v=0) : val(v) {}
private: int val;
} f[2][N];
int main() {
    freopen("sing.in", "r", stdin);
    freopen("sing.out", "w", stdout);
    int n, k;
    scanf("%d%d", &n, &k);
    for (int i = 1; i <= n; i++) scanf("%d", &b[i]);
    std::sort(b+1, b+1+n);
    auto fnow = f[0], fnxt = f[1];
    fnow[0] = 1;
    for (int i = 1; i <= n; i++) {
        memset(fnxt, 0, sizeof(f) / 2);
        for (int j = 0; j <= k; j++) {
            if (b[i] >= j) fnxt[j+1] += fnow[j] * (std::min(b[i], k) - j + 1), fnxt[j] += fnow[j] * (j + std::max(0, b[i] - k));
            else fnxt[j] += fnow[j] * (b[i] + 1);
        }
        fnxt[k+1] += fnow[k+1] * (b[i] + 1);
        std::swap(fnow, fnxt);
    }
    printf("%d\n", fnow[k+1]);
}