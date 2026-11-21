#include <bits/stdc++.h>
constexpr int N = 300005;
#ifdef CLANGD
#define N 35
#endif
std::vector<std::pair<int,int>> g[N];
int fa[N], rep[N];
int64_t dis[N], mxdep;
int mxdepidx;
inline int find(int u) { return u == fa[u] ? u : (fa[u] = find(fa[u])); }
void dfs(int u, int fa, int64_t d) {
    if (d > mxdep) mxdep = d, mxdepidx = u;
    for (auto [v, w] : g[u]) if (v != fa) dfs(v, u, d + w);
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
        std::iota(rep+1, rep+1+n, 1);
        memset(dis, 0, sizeof(int64_t) * (n+1));
        for (int i = 1; i <= n; i++) g[i].clear();
        for (auto &[w, u, v] : edgs) {
            int fu = find(u), fv = find(v);
            assert(fu != fv);
            fa[fu] = fv;
            u = rep[fu], v = rep[fv];
            g[rep[fu]].emplace_back(rep[fv], w);
            g[rep[fv]].emplace_back(rep[fu], w);
            if (dis[fu] < dis[fv]) dis[fv] = std::max(dis[fv], dis[fu] + w);
            else dis[fv] = std::max(dis[fu], dis[fv] + w), rep[fv] = rep[fu];
        }
        mxdep = 0, dfs(1, 0, 0);
        mxdep = 0, dfs(mxdepidx, 0, 0);
        printf("%lld\n", mxdep);
        for (const auto &[w, u, v] : edgs)
            printf("%d %d %d\n", u, v, w);
    }
}