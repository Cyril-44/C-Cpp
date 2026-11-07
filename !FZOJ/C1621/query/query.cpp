#include <bits/stdc++.h>
constexpr int N = 505;
constexpr static int MOD = 998244353;
struct Mint {
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
    Mint pow(int n) const {
        Mint res = 1, b = *this;
        for (; n; n >>= 1, b *= b)
            if (n & 1) res *= b;
        return res;
    }
    Mint(int v = 0) : val(v) {}

  private:
    int val;
} f[2][N][N], C[N][N], A[N][N], fac[N], ifac[N];
int h[N];
int main() {
    int n, op;
    scanf("%d%d", &n, &op);
    for (int i = 0; i <= n; i++) {
        C[i][0] = C[i][i] = 1;
        for (int j = 1; j < i; j++) C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
    }
    for (int i = 0; i <= n; i++) {
        A[i][0] = 1;
        for (int j = 1; j <= n; j++)
            A[i][j] = A[i][j-1] * (i-j+1);
    }
    fac[0] = 1; for (int i = 1; i <= n; i++) fac[i] = fac[i-1] * i;
    ifac[n] = fac[n].pow(MOD-2); for (int i = n; i >= 1; i--) ifac[i-1] = ifac[i] * i;
    for (int i = 1; i <= n; i++) {
        int ai;
        scanf("%d", &ai);
        ++h[ai];
    }
    for (int i = n; i >= 1; i--) h[i] += h[i+1];
    auto fnow = f[0], fnxt = f[1];
    fnow[0][0] = 1;
    if (op == 1) {
        for (int i = n; i >= 1; i--) {
            memset(fnxt, 0, sizeof(f) / 2);
            for (int j = 0; j <= n; j++)
                for (int k = (i+1) * j; k <= h[i]; k++)
                    for (int l = 0; k+i*l <= n; l++) {
                        if (n-j < l || h[i]-k < i*l) continue;
                        fnxt[j + l][k + i*l] += fnow[j][k] * C[n-j][l] * A[h[i]-k][i*l] * ifac[i].pow(l);
                    }
            std::swap(fnxt, fnow);
        }
    } else {
        for (int i = n; i >= 1; i--) {
            memset(fnxt, 0, sizeof(f) / 2);
            for (int j = 0; j <= h[i]; j++)
                for (int k = (i+1) * j; k <= h[i]; k++)
                    for (int l = 0; k+i*l <= n && l <= n-j; l++) {
                        if (h[i]-j < l || h[i]-k < i*l) continue;
                        fnxt[j + l][k + i*l] += fnow[j][k] * C[h[i]-j][l] * A[h[i]-k][i*l] * ifac[i].pow(l);
                    }
            std::swap(fnxt, fnow);
        }
    }
    Mint ans = 0;
    for (int i = 1; i <= n; i++) ans += fnow[i][n];
    printf("%d\n", ans);
}