#include <cstdio>
#include <cstring>
#define For(i, s, t) for (int i = (s); i <= (t); i++)
#define roF(i, s, t) for (int i = (s); i >= (t); i--)
constexpr int N = 2005, MOD = (int)1e9 + 7;
struct Mint {
    Mint(int v=0) : val(v) {}
    Mint& operator+=(Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint x, Mint y) { return x += y; }
    friend Mint operator-(Mint x, Mint y) { return x -= y; }
    friend Mint operator*(Mint x, Mint y) { return x *= y; }
    template<typename T> explicit operator T() const { return static_cast<T>(val); }
private: int val;
} inv[N], C[N][N], nC[N][N];
inline Mint nCm(int n, int m) {
    Mint res = 1;
    For(i, 1, m) res *= (m-i+1) * inv[i];
    return res;
}
inline Mint qpow(Mint b, int n) {
    Mint res = 1;
    while (n) {
        if (n & 1) res *= b;
        b *= b, n >>= 1;
    }
    return res;
}
int n, m;
inline void sub1() {
    Mint ans = 0;
    for (int s = 0; s < (1<<n); s++) {
        if (n - __builtin_popcount(s) & 1) ans -= qpow(s, m);
        else ans += qpow(s, m);
    }
    printf("%d\n", ans);
}
inline void sub2() {
    static Mint f[N], pwk[N];
    Mint base = 1;
    f[0] = 1;
    For(i, 1, n) {
        pwk[0] = 1;
        For(j, 1, m-1) pwk[j] = pwk[j-1] * base;
        roF(j, m-1, 0) {
            f[j+1] = 0;
            For(k, 1, m-j) 
                f[j+k] += f[j] * nC[j][k] * pwk[k];
        }
        f[0] = 0;
        base += base;
    }
    printf("%d\n", f[m]);
}
int main() {
    inv[1] = 1;
    For(i, 2, 2000) inv[i] = (MOD - MOD / i) * inv[MOD % i];
    For(i, 0, 2000) {
        C[i][0] = C[i][i] = 1;
        For(j, 1, i-1) C[i][j] = C[i-1][j-1] + C[i-1][j];
    }
    For(i, 0, 2000)
        For(j, 0, 2000-i)
            nC[i][j] = C[i+j][j];
    scanf("%d%d", &n, &m);
    if (n <= 20) sub1();
    else sub2();
    return 0;
}