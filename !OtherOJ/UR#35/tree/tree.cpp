#include <bits/stdc++.h>
constexpr int N = 300005;
#ifdef CLANGD
#define N 35
#endif
std::vector<std::pair<int,int>> g[N];
int fa[N];
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
        std::set<std::tuple<int,int,int>> nedgs;
        for (auto &[w, u, v] : edgs) scanf("%d%d%d", &u, &v, &w);
        std::sort(edgs.begin(), edgs.end());
        std::iota(fa+1, fa+1+n, 1);
        memset(dis, 0, sizeof(int64_t) * (n+1));
        for (int i = 1; i <= n; i++) g[i].clear();
        std::vector<decltype(edgs)::iterator> tmp;
        int lastw = 0;
        auto work = [&]() {
            for (auto it : tmp) {
                auto &[w, u, v] = *it;
                int ru = find(u), rv = find(v);
                if (dis[ru] < dis[rv]) fa[ru] = rv;
                else fa[rv] = ru;
            }
            for (auto it : tmp) {
                auto &[w, u, v] = *it;
                int rt = find(u);
                if (u != rt) nedgs.emplace(w, u, rt), dis[rt] = std::max(dis[rt], dis[u] + w);
                if (v != rt) nedgs.emplace(w, v, rt), dis[rt] = std::max(dis[rt], dis[v] + w);
            }
            tmp.clear();
        };
        for (auto it = edgs.begin(); it != edgs.end(); ++it) {
            auto &[w, u, v] = *it;
            if (w != lastw) work();
            u = find(u), v = find(v);
            tmp.emplace_back(it);
            lastw = w;
        }
        work();
        for (const auto &[w, u, v] : nedgs) {
            g[u].emplace_back(v, w);
            g[v].emplace_back(u, w);
        }
        mxdep = 0, dfs(1, 0, 0);
        mxdep = 0, dfs(mxdepidx, 0, 0);
        printf("%lld\n", mxdep);
        for (const auto &[w, u, v] : nedgs)
            printf("%d %d %d\n", u, v, w);
    }
}