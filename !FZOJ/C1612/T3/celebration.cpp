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
constexpr int N = 5;
#else
constexpr int N = 1000005;
#endif
std::vector<int> g[N];
int a[N], n, m, k;
struct LCA {
    constexpr static int K = 19;
    int st[N][K+1], dep[N], dfn[N], top;
    void dfs(int u, int fa) {
        st[dfn[u] = ++top][0] = fa;
        for (int v : g[u]) if (v != fa)
            dep[v] = dep[u] + 1, dfs(v, u);
    }
    int min(int u, int v) const {
        return dfn[u] < dfn[v] ? u : v;
    }
    void init() {
        dfs(1, 0);
        for (int k = 1; k <= K; k++)
            for (int i = 1; i + (1<<k) - 1 <= n; i++)
                st[i][k] = min(st[i][k-1], st[i + (1<<k-1)][k-1]);
    }
    int operator()(int u, int v) const {
        if (u == v) return u;
        u = dfn[u], v = dfn[v];
        if (u > v) std::swap(u, v);
        int k = 31 - __builtin_clz(v - u++);
        return min(st[u][k], st[v - (1<<k) + 1][k]);
    }
} lca;
using DPNode = std::pair<int64_t, int>;
struct StaNode { DPNode f; int d; } sta[N];
int d[N];
DPNode f[N], premn[N];
int main() {
    in(n), in(m), in(k);
    for (int i = 1; i <= m; i++) in(a[i]);
    for (int u, v, i = 1; i < n; i++) {
        in(u), in(v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    lca.init();
    for (int i = 1; i < m; i++) d[i] = lca.dep[lca(a[i], a[i+1])];
    int64_t ans = 1ll << 60;
    int l = 0, r = n;
    while (l <= r) {
        int lambda = l + r >> 1;
        int top = 0;
        premn[0] = {1ll<<60, 0};
        for (int i = 1; i <= m; i++) {
            f[i] = {f[i-1].first + lca.dep[a[i]], f[i-1].second+1};
            if (i >= 2) {
                DPNode mnf = f[i-2];
                for (; top && sta[top].d >= d[i-1]; top--)
                    mnf = std::min(mnf, sta[top].f);
                sta[++top] = {mnf, d[i-1]};
                premn[top] = std::min(premn[top-1], {mnf.first + d[i-1], mnf.second});
                f[i] = std::min(f[i], {premn[top].first, premn[top].second + 1});
            }
            f[i].first -= lambda;
        }
        if (f[m].second <= k) ans = f[m].first + 1ll * lambda * k, l = lambda + 1;
        else r = lambda - 1;
    }
    printf("%lld\n", ans);
}