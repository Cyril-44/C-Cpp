#include <bits/stdc++.h>
namespace FastI {
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
} using FastI::in;
#ifdef CLANGD
constexpr int N = 15;
#else
constexpr int N = 1000005;
#endif
struct Range {
    int l, r;
} rg[N>>1];
int u[N];
struct Mint {
    constexpr static int MOD = (int)1e9 + 7;
    Mint& operator+=(Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint x, Mint y) { return x += y; }
    friend Mint operator-(Mint x, Mint y) { return x -= y; }
    friend Mint operator*(Mint x, Mint y) { return x *= y; }
    Mint (int v=0) : val(v) {}
    template<typename T> explicit operator T() const { return static_cast<T>(val); }
private: int val;
} pw2[N];
int cov[N], cnt[N];
int m;
int main() {
    int n; in(n);
    pw2[0] = 1;
    for (int i = 1; i <= n; i++) pw2[i] = pw2[i-1] + pw2[i-1];
    for (int i = 1; i <= n; i++) {
        in(rg[i].l), in(rg[i].r);
        u[i-1<<1] = rg[i].l, u[i-1<<1|1] = rg[i].r;
    }
    std::sort(u, u+2*n);
    m = std::unique(u, u+2*n) - u;
    for (int i = 1; i <= n; i++) {
        rg[i].l = std::lower_bound(u, u+m, rg[i].l) - u+1;
        rg[i].r = std::lower_bound(u, u+m, rg[i].r) - u+1;
        ++cnt[rg[i].l], ++cov[rg[i].l], --cov[rg[i].r+1];
    }
    for (int i = 1; i <= m; i++) cov[i] += cov[i-1];
    Mint ans = 0;
    for (int i = 1; i <= m; i++)
        ans += (pw2[cnt[i]] - 1) * (pw2[n - cov[i]]);
    printf("%d\n", ans);
}