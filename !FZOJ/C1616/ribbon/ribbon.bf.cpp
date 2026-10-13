#include <bits/stdc++.h>
constexpr int N = 805, MOD = 998244353;
struct Mint {
    Mint& operator+=(Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    Mint pow(int n) const {
        Mint b = *this, res = 1;
        while (n) {
            if (n & 1) res *= b;
            b *= b, n >>= 1;
        }
        return res;
    }
    friend Mint operator+(Mint x, Mint y) { return x += y; }
    friend Mint operator-(Mint x, Mint y) { return x -= y; }
    friend Mint operator*(Mint x, Mint y) { return x *= y; }
    Mint(int v=0) : val(v) {}
private: int val;
} g[N];
int p[N], c[N], h[N], th[N], d[N];
int64_t f[N];
int main() {
    int n; scanf("%d", &n);
    if (n > 800) {
        for (int i = 1; i <= n; i++) scanf("%*d");
        int cnt0 = 0, c1; scanf("%d", &c1);
        for (int i = 2, ci; i <= n; i++) scanf("%d", &ci), cnt0 += !ci;
        printf("%d %d\n", c1, Mint(2).pow(cnt0));
        return 0;
    }
    for (int i = 1; i <= n; i++) scanf("%d", &p[i]);
    for (int i = 1; i <= n; i++) scanf("%d", &c[i]);
    for (int i = 1; i <= n; i++) {
        memset(h, 0, sizeof h);
        h[p[i]] = n+1, h[n+1] = p[i];
        for (int j = i+1; j <= n; j++) {
            memset(th, 0, sizeof th);
            for (int v = 1; v <= n + 1; v++) {
                if (p[j] > v) th[p[j]] = std::max(th[p[j]], h[v]);
                if (p[j] < h[v]) th[v] = std::max(th[v], p[j]);
            }
            bool brk = true;
            for (int v = 1; v <= n + 1; v++)
                if (th[v]) { brk = false; break; }
            if (brk) { d[i] = j; break; }
            memcpy(h, th, sizeof th);
        }
        if (!d[i]) d[i] = n+1;
    }
    memset(f, 0x3f, sizeof f);
    f[1] = 0;
    for (int i = 1; i <= n; i++)
        for (int j = i+1; j <= d[i]; j++)
            f[j] = std::min(f[j], f[i] + c[i]);
    g[1] = 1;
    for (int i = 1; i <= n; i++)
        for (int j = i+1; j <= d[i]; j++)
            if (f[i] + c[i] == f[j]) g[j] += g[i];
    printf("%lld %d\n", f[n+1], g[n+1]);
}