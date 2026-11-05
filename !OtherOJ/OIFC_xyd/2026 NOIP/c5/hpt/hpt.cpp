#include <bits/stdc++.h>
constexpr int N = 500005, K = 18;
#ifdef CLANGD
#define N 55
#endif
std::vector<int> g[N];
int a[N], dfn[N], st[N][K+1];
int64_t f[N], fs[N], ff[N];
void dfs(int u, int fa) {
    f[u] = a[u], dfn[u] = ++dfn[0], st[dfn[u]][0] = fa;
    for (int v : g[u]) if (v != fa) {
        dfs(v, u);
        if (f[v] > 0) f[u] += f[v];
    }
}
void dfs2(int u, int fa) {
    for (int v : g[u]) if (v != fa) {
        fs[v] = fs[u] + f[u] - std::max(f[v], (int64_t)0);
        ff[v] = std::max((int64_t)0, ff[u] + f[u] - std::max(f[v], (int64_t)0));
        dfs2(v, u);
    }
}
inline int min(int x, int y) { return dfn[x] < dfn[y] ? x : y; }
inline int lca(int x, int y) {
    if (x == y) return x;
    if ((x=dfn[x]) > (y=dfn[y])) std::swap(x, y);
    const int k = std::__lg(y - x++);
    return min(st[x][k], st[y - (1<<k) + 1][k]);
}
int main() {
    freopen("hpt.in", "r", stdin);
    freopen("hpt.out", "w", stdout);
    int T, n, m;
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d", &n, &m);
        for (int i = 1; i <= n; i++)
            scanf("%d", &a[i]);
        for (int i = 1; i <= n; i++) g[i].clear();
        for (int i = 1, u, v; i < n; i++) {
            scanf("%d%d", &u, &v);
            g[u].push_back(v);
            g[v].push_back(u);
        }
        dfn[0] = 0, dfs(1, 0), dfs2(1, 0);
        for (int k = 1; k <= K; k++)
            for (int i = 1; i + (1<<k) - 1 <= n; i++)
                st[i][k] = min(st[i][k-1], st[i + (1<<k-1)][k-1]);
        int64_t ans = *std::max_element(f+1, f+1+n);
        for (int i = 1, u, v; i <= m; i++) {
            int64_t w; scanf("%d%d%lld", &u, &v, &w);
            int lc = lca(u, v);
            if (lc == v) std::swap(u, v);
            if (lc == u) ans = std::max(ans, w + f[v] + fs[v] - fs[u] + ff[u]);
            else ans = std::max(ans, w + f[u] + f[v] + fs[u] + fs[v] - f[lc] - 2 * fs[lc] + ff[lc]);
        }
        printf("%lld\n", ans);
    }
}