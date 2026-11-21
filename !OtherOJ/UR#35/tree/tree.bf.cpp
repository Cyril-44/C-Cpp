#include <bits/stdc++.h>
constexpr int N = 300005;
#ifdef CLANGD
#define N 35
#endif
std::vector<std::pair<int,int>> g[N];
int fa[N], pre[N], prew[N];
int64_t mxdep;
int mxdepidx;
inline int find(int u) { return u == fa[u] ? u : (fa[u] = find(fa[u])); }
void dfs(int u, int64_t d) {
    if (d > mxdep) mxdep = d, mxdepidx = u;
    for (auto [v, w] : g[u]) if (v != pre[u]) prew[v] = w, pre[v] = u, dfs(v, d + w);
}
inline void getdiam(int u) {
    mxdep = 0, pre[u] = 0, dfs(u, 0);
    mxdep = 0, pre[mxdepidx] = 0, dfs(mxdepidx, 0);
}
int main() {
    int T, n;
    scanf("%d", &T);
    while (T--) {
        scanf("%d", &n);
        std::vector<std::tuple<int,int,int>> edgs(n-1);
        for (auto &[w, u, v] : edgs) scanf("%d%d%d", &u, &v, &w);
        std::sort(edgs.begin(), edgs.end());
        std::iota(fa+1, fa+1+n, 1);
        for (int i = 1; i <= n; i++) g[i].clear();
        for (auto &[w, u, v] : edgs) {
            int fu = find(u), fv = find(v);
            int64_t mn = 1e18;
            for (int i = 1; i <= n; i++)
                for (int j = 1; j <= n; j++)
                    if (find(i) == fu && find(j) == fv) {
                        g[i].emplace_back(j, w);
                        g[j].emplace_back(i, w);
                        getdiam(i);
                        if (mxdep < mn) mn = mxdep, u = i, v = j;
                        g[i].pop_back();
                        g[j].pop_back();
                    }
            fa[fu] = fv;
            g[u].emplace_back(v, w);
            g[v].emplace_back(u, w);
            
        }
        getdiam(1);
        printf("%lld\n", mxdep);
        for (const auto &[w, u, v] : edgs)
            printf("%d %d %d\n", u, v, w);
    }
}