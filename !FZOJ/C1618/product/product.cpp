#include <bits/stdc++.h>
constexpr int N = 100005, M = 101, MOD = (int)1e9 + 7;
char s[N];
int h[10];
struct Mint {
    Mint& operator+=(Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint x, Mint y) { return x += y; }
    friend Mint operator-(Mint x, Mint y) { return x -= y; }
    friend Mint operator*(Mint x, Mint y) { return x *= y; }
    template<class T> explicit operator T() const { return val; }
    Mint(int v=0) : val(v) {}
private: int val;
} f[M][M * 10], C[M][M], inv[M], ans;
inline Mint Com(int n, int m) {
    if (n < m) return 0;
    Mint res = 1;
    for (int i = 1; i <= m; i++) res *= (n-i+1) * inv[i];
    return res;
}
int n, m;
constexpr int LMT = 1000;
int main() {
    scanf("%d%d %s", &n, &m, s+1);
    inv[1] = 1;
    for (int i = 2; i <= m; i++) inv[i] = (MOD - MOD / i) * inv[MOD % i];
    for (int i = 0; i <= m; i++) {
        C[i][0] = C[i][i] = 1;
        for (int j = 1; j < i; j++)
            C[i][j] = C[i-1][j-1] + C[i-1][j];
    }
    for (int i = 1; i <= n; i++) h[s[i] - '0']++;
    for (int i = 1, last = 0; i <= n; i++)
        ans += Com(n - last - 1, m - 1);
    ans -= Com(h[0], m-1);
    for (int v = 1; v <= 9; v++)
        for (int k = m; k >= 1; k--)
            for (int i = 0; i < m*10; i++) if (f[k-1][i])
                for (int j = i+1; j <= m*10 && j <= i + h[v]; j++)
                    f[k][j] += f[k-1][i] * C[h[v]][j-i];
    int count = 0;
    for (int i2 = 0, b2 = 1; i2 <= 9; i2++, b2 *= 2)
        for (int i3 = 0, b3 = 1; i3 <= 6; i3++, b3 *= 3)
            for (int i5 = 0, b5 = 1; i5 <= 5; i5++, b5 *= 5)
                for (int i7 = 0, b7 = 1; i7 <= 3; i7++, b7 *= 7) {
                    int val = b2 * b3 * b5 * b7, cnt = i2 + i3 + i5 + i7;
                    if (val > LMT) break;
                    int rem = val - i2 * 2 - i3 * 3 - i5 * 5 - i7 * 7;
                    ++count;
                }
    fprintf(stderr, "%d\n", count);
}