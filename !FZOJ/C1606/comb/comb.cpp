#include <cstdio>
constexpr int MOD = (int)1e9 + 7;
constexpr int N = 2001;
struct Mint {
    Mint& operator+=(Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint x, Mint y) { return x += y; }
    friend Mint operator-(Mint x, Mint y) { return x -= y; }
    friend Mint operator*(Mint x, Mint y) { return x *= y; }
    Mint(int v=0) : val(v) {}
    explicit operator int() const { return val; }
private: int val;
} pw2[N], C[N][N], f[N][N];
inline void in(int &x) {
    char ch = getchar();
    while (ch < '0' || ch > '9') ch = getchar();
    for (x = 0; ch >= '0' && ch <= '9'; ch = getchar())
        x = (x << 3) + (x << 1) + (ch ^ '0');
}
inline void out(int x) {
    static int sta[20];
    sta[sta[0] = 1] = x % 10;
    while (x /= 10) sta[++sta[0]] = x % 10;
    while (sta[0]) putchar(sta[sta[0]--] | '0');
    putchar('\n');
}
int main() {
    int n, q; in(n), in(q);
    pw2[0] = 1;
    for (int i = 1; i <= n; i++)
        pw2[i] = pw2[i-1] + pw2[i-1];
    for (int i = 1; i <= n; i++) {
        C[i][0] = C[i][i] = 1;
        for (int j = 1; j < i; j++)
            C[i][j] = C[i-1][j-1] + C[i-1][j];
    }
    for (int c = 1; c <= n; c++)
        for (int i = 1; i <= c; i++)
            f[c][i] = f[c][i-1] + pw2[c-i] * i * i * (c - i) * C[c][i];
    for (int a, b, c; q--; ) {
        in(a), in(b), in(c);
        out(int(f[c][b] - f[c][a-1]));
    }
    return 0;
}