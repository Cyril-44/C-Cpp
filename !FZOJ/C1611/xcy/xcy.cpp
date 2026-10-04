#include <bits/stdc++.h>
char buf[1 << 20], *p1=buf, *p2=buf;
inline char gc() {
    if (p1 == p2) {
        p2 = (p1=buf) + fread(buf, 1, sizeof buf, stdin);
        if (p1 == p2) return EOF;
    }
    return *p1++;
}
template<class T> inline void in(T &x) {
    char ch = gc();
    while (ch < '0' || ch > '9') ch = gc();
    for (x = 0; ch >= '0' && ch <= '9'; ch = gc())
        x = (x << 3) + (x << 1) + (ch ^ '0');
}
#ifdef CLANGD
constexpr int N = 2;
#else
constexpr int N = 5005;
#endif
#define For(i, s, t) for (int i = (s); i <= int(t); i++)
#define roF(i, s, t) for (int i = (s); i >= int(t); i--)
constexpr int M1 = (int)1e9 + 7, M2 = (int)1e9 + 6;
template<int MOD>
struct Mint {
    Mint& operator+=(Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint x, Mint y) { return x += y; }
    friend Mint operator-(Mint x, Mint y) { return x -= y; }
    friend Mint operator*(Mint x, Mint y) { return x *= y; }
    Mint pow(unsigned n = MOD-2) {
        Mint res = 1, b = *this;
        while (n) {
            if (n & 1) res *= b;
            b *= b, n >>= 1;
        }
        return res;
    }
    Mint (int v=0) : val(v) {}
    template<typename T> explicit operator T() const { return static_cast<T>(val); }
private: int val;
};
Mint<M2> C[N][N], blks[N];
int cnt[N];
int main() {
    // freopen("xcy.in", "r", stdin);
    // freopen("xcy.out", "r", stdout);
    int n, k, q;
    in(n), in(k), in(q);
    int m = (n+1) / 2;
    For(i, 0, m+k) {
        C[i][0] = C[i][i] = 1;
        For(j, 1, i-1) C[i][j] = C[i-1][j-1] + C[i-1][j];
    }
    For(i, 0, m) blks[i] = C[i + k][k];
    roF(i, m, 1) blks[i] -= blks[i-1];
    For(d, 1, m) cnt[d]++;
    for (int ai, bi; q--; ) {
        int l=1;
        For(i, 1, k) in(ai), l = std::max(l, ai);
        For(i, 1, k) in(bi), l = std::max(l, n-bi+1);
        For(d, l, m) cnt[d]++;
        Mint<M1> ans = 1;
        For(d, 1, m) ans *= Mint<M1>(cnt[d]).pow(unsigned(blks[m-d]));
        printf("%d\n", ans);
    }
}