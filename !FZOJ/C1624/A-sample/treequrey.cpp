#include <bits/stdc++.h>
constexpr int N = 250005;
#ifdef CLANGD
#define N 25
#endif
std::vector<int> g[N];
struct DSU {
    int fa[N], sz[N];
    void reset(int i) { fa[i] = i, sz[i] = 1; }
    int find(int u) { return u == fa[u] ? u : (fa[u] = find(fa[u])); }
    void unite(int u, int v) {
        if ((u=find(u)) == (v=find(v))) return;
        if (sz[u] < sz[v]) std::swap(u, v);
        fa[v] = u, sz[u] += sz[v];
    }
} dsu;
int fa[N], a[N];
bool vis[N];
void dfs(int u) {
    for (int v : g[u]) if (v != fa[u])
        fa[v] = u, dfs(v);
}
inline int64_t com2(int v) { return 1ll * v * (v-1) / 2; }
int main() {
    int n, q;
    scanf("%d", &n);
    for (int a, b, i = 1; i < n; i++) {
        scanf("%d%d", &a, &b);
        g[a].push_back(b);
        g[b].push_back(a);
    }
    dfs(1);
    for (int i = 1; i <= n; i++) dsu.reset(i);
    scanf("%d", &q);
    for (int k; q--; ) {
        scanf("%d", &k);
        for (int i = 1; i <= k; i++)
            scanf("%d", &a[i]), vis[a[i]] = true;
        for (int i = 1; i <= k; i++)
            if (vis[fa[a[i]]]) dsu.unite(a[i], fa[a[i]]);
        int64_t ans = 0;
        for (int i = 1; i <= k; i++)
            if (dsu.find(a[i]) == a[i])
                ans += com2(dsu.sz[a[i]]);
        for (int i = 1; i <= k; i++)
            vis[a[i]] = false, dsu.reset(a[i]);
        printf("%lld\n", ans);
    }
}