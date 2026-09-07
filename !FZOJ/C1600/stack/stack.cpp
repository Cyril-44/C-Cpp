#include <cstdio>
constexpr int MOD = 998244353, M = 300005;
struct Mint {
    Mint& operator+=(Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint x, Mint y) { return x += y; }
    friend Mint operator-(Mint x, Mint y) { return x -= y; }
    friend Mint operator*(Mint x, Mint y) { return x *= y; }
    Mint(int v=0) : val(v) {}
    int val;
} f[M];
inline Mint qpow(Mint b, int n) {
    Mint res = 1;
    for (; n; b *= b, n >>= 1)
        if (n & 1) res *= b;
    return res;
}
inline Mint inv(Mint b) { return qpow(b, MOD-2); }
int main() {
    int T, n, m;
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d", &n, &m);
        Mint mn = qpow(m, n); Mint invm = inv(m);
        for (int i = 2; i <= m; i++) { // f[i] = sum_(j=1)^n (m-i+1)^j m^(n-j)
            Mint q = (m - i + 1) * invm;
            f[i] = mn * (qpow(q, n+1) - q) * inv(q - 1);
        }
        f[1] = n * mn;
        for (int i = 1; i < m; i++) f[i] -= f[i+1];
        Mint ans = 0;
        for (int i = 1; i <= m; i++) ans += f[i] * i;
        printf("%d\n", ans);
    }
    return 0;
}