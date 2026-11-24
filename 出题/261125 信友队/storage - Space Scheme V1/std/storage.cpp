#include <bits/stdc++.h>
constexpr int N = 100005, MOD = 998244353;
struct Mint {
    Mint& operator+=(const Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(const Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(const Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint x, const Mint y) { return x += y; }
    friend Mint operator-(Mint x, const Mint y) { return x -= y; }
    friend Mint operator*(Mint x, const Mint y) { return x *= y; }
    Mint(int v=0) : val(v) {}
private: int val;
};
using Info = std::pair<int, Mint>;
Info operator+(Info x, Info y) { return {x.first + y.first, x.second * y.second}; }
Info operator+(Info x, int y) { return {x.first + y, x.second}; }
Info min(Info x, Info y) {
    if (x.first < y.first) return x;
    if (x.first > y.first) return y;
    return {x.first, x.second + y.second};
}
std::pair<int, Mint> f[N << 2], g[N << 2];
int c[N << 2], L, R, X;
inline void pushup(int u) {
    g[u] = min(f[u<<1] + f[u<<1|1] + 1, g[u<<1] + g[u<<1|1]);
    if (c[u] > 0) f[u] = g[u];
    else f[u] = f[u<<1] + f[u<<1|1];
}
void update(int u, int l, int r) {
    if (l == r) {
        c[u] += X;
        f[u] = {(c[u] > 0), 1};
        g[u] = {1, 1};
        return;
    }
    if (L <= l && r <= R) {
        c[u] += X;
        return pushup(u);
    }
    int mid = l + r >> 1;
    if (L <= mid) update(u<<1, l, mid);
    if (mid < R) update(u<<1|1, mid+1, r);
    pushup(u);
}
void build(int u, int l, int r) {
    if (l == r) f[u] = {0, 1}, g[u] = {1, 1};
    else {
        int mid = l + r >> 1;
        build(u<<1, l, mid);
        build(u<<1|1, mid+1, r);
        pushup(u);
    }
}
int main() {
    freopen("storage.in", "r", stdin);
    freopen("storage.out", "w", stdout);
    int Tid, n, q;
    scanf("%d%d%d", &Tid, &n, &q);
    build(1, 1, n);
    char ch;
    while (q--) {
        scanf(" %c%d%d", &ch, &L, &R);
        X = ch == '+' ? 1 : -1;
        update(1, 1, n);
        printf("%d %d\n", f[1].first, f[1].second);
    }
}