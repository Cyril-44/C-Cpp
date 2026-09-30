#include <cstdio>
#include <vector>
#include <algorithm>
#ifdef CLANGD
constexpr int N = 35;
#else
constexpr int N = 300005;
#endif
std::vector<std::pair<int,int>> g[N];
int dep[N], sz[N], hson[N], dfn[N], top[N], fa[N], faw[N];
int64_t f[N], fh[N], fup[N], fssum[N], fpsum[N], fpmn[N];
void dfs1(int u) {
    sz[u] = 1;
    for (auto [v, w] : g[u]) if (v != fa[u]) {
        dep[v] = dep[u] + 1;
        fa[v] = u, faw[v] = w;
        dfs1(v);
        f[u] += fup[v] = std::max(f[v] + w, (int64_t)0);
        sz[u] += sz[v];
        if (sz[v] > sz[hson[u]])
            hson[u] = v;
    }
}
void dfs2(int u) {
    dfn[u] = ++dfn[0];
    if (!top[u]) top[u] = u;
    if (int hv = hson[u]) {
        fh[u] = f[u] - fup[hv];
        top[hv] = top[u];
        fpsum[dfn[u]+1] = fpsum[dfn[u]] + faw[hv] + fh[u];
        fpmn[dfn[u]+1] = std::min(fpmn[dfn[u]], fpsum[dfn[u]+1]);
        dfs2(hv);
        fssum[dfn[u]] = fssum[dfn[u]+1] + faw[hv] + fh[u];
        for (auto [v, w] : g[u]) if (v != fa[u] && v != hv)
            dfs2(v);
    }
}
int64_t inquire(int u, int v) {
    int64_t sum = 0, ans = 0;
    for (; top[u] != top[v]; u = fa[top[u]]) { // u --> fa[top[u]]
        if (dep[top[u]] < dep[top[v]]) std::swap(u, v);
        sum += f[u] + fssum[dfn[top[u]]] - fssum[dfn[u]] + faw[top[u]] - fup[top[u]];
    }
    if (dep[u] < dep[v]) std::swap(u, v); // v 为 lca
    ans = sum = sum + f[u] + fssum[dfn[v]] - fssum[dfn[u]];
    for (; v; v = fa[top[v]]) { // v --> fa[top[v]]
        ans = std::max(ans, sum + fpsum[dfn[v]] - fpmn[dfn[v]]);
        sum += fpsum[dfn[v]] + faw[top[v]] + f[fa[top[v]]] - fup[top[v]];
    }
    return ans;
}
int main() {
    int n, q;
    scanf("%d", &n);
    for (int i = 1, u, v, w; i < n; i++) {
        scanf("%d%d%d", &u, &v, &w);
        g[u].emplace_back(v, w);
        g[v].emplace_back(u, w);
    }
    dfs1(1), dfs2(1);
    scanf("%d", &q);
    for (int s, t; q--; ) {
        scanf("%d%d", &s, &t);
        printf("%lld\n", inquire(s, t));
    }
    return 0;
}