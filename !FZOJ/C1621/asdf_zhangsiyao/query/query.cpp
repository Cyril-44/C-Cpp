#include <bits/stdc++.h>
constexpr int N = 505;
struct Mint {
    constexpr static int MOD = 998244353;
    Mint &operator+=(const Mint o) {
        if ((val += o.val) >= MOD) val -= MOD;
        return *this;
    }
    Mint &operator-=(const Mint o) {
        if ((val -= o.val) < 0) val += MOD;
        return *this;
    }
    Mint &operator*=(const Mint o) {
        val = 1ull * val * o.val % MOD;
        return *this;
    }
    friend Mint operator+(Mint x, const Mint y) { return x += y; }
    friend Mint operator-(Mint x, const Mint y) { return x -= y; }
    friend Mint operator*(Mint x, const Mint y) { return x *= y; }
    Mint(int v = 0) : val(v) {}

  private:
    int val;
} C[N][N];
int a[N];
int main() {
    freopen("query.in", "r", stdin);
    freopen("query.out", "w", stdout);
    int n, op;
    scanf("%d%d", &n, &op);
    for (int i = 0; i <= n; i++) {
        C[i][0] = C[i][i] = 1;
        for (int j = 1; j < i; j++) C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
    }
    for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
    if (op == 1) {
        Mint f[2][N]{};
        auto fpre = f[0], fnow = f[1];
        fnow[0] = 1;
        for (int i = 1; i <= n; i++) {
            std::swap(fpre, fnow);
            memset(fnow, 0, sizeof(Mint) * (n + 1));
            for (int j = 0; j <= n; j++)
                for (int k = 0; k <= a[i] && k <= j; k++) fnow[j] += fpre[j - k] * C[j][k];
        }
        printf("%d\n", fnow[n]);
    }
}