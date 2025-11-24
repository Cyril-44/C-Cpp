#include <stdio.h>
#include <vector>
#include <numeric>
#include <algorithm>
struct DSU {
    std::vector<int> fa;
    inline DSU(int n) : fa(n+1) { std::iota(fa.begin(), fa.end(), 0); }
    inline int getfa(int u) {
        int v = u, tp;
        while (u ^ fa[u]) u = fa[u];
        while (v ^ u) tp = fa[v], fa[v] = u, v = tp;
        return u;
    }
    inline bool merge(int u, int v) {
        u = getfa(u), v = getfa(v);
        if (u ^ v) return fa[u] = v;
        return false;
    }
};
constexpr int N = 100005, M = 300005, K = 17;
std::pair<int, std::pair<int,int>> edges[M];
std::vector<std::pair<int,int>> g[N];
int fa[N][K + 1], mx[N][K + 1], dep[N];
void dfs(int u, int pre) {
    fa[u][0] = pre;
    for (int k = 1; k <= K; k++)
        fa[u][k] = fa[fa[u][k-1]][k-1];
    for (int k = 1; k <= K; k++)
        mx[u][k] = std::max(mx[u][k-1], mx[fa[u][k-1]][k-1]);
    for (const auto &edg : g[u]) {
#define v edg.first
#define w edg.second
        if (v ^ pre) {
            dep[v] = dep[u] + 1;
            mx[v][0] = w;
            dfs(v, u);
        }
#undef v
#undef w
    }
}
inline int lca(int u, int v) {
    if (dep[u] < dep[v]) std::swap(u, v);
    for (int tp = dep[u] - dep[v], k = K; k >= 0; k--)
        if (tp >> k & 1) u = fa[u][k];
    if (u == v) return u;
    for (int k = K; k >= 0; k--)
        if (fa[u][k] ^ fa[v][k])
            u = fa[u][k], v = fa[v][k];
    return fa[u][0];
}
inline int getmx(int u, int v) {
    int com = lca(u, v), ans = 0;
    for (int tp = dep[u] - dep[com], k = K; k >= 0; k--)
        if (tp >> k & 1) ans = std::max(ans, mx[u][k]), u = fa[u][k];
    for (int tp = dep[v] - dep[com], k = K; k >= 0; k--)
        if (tp >> k & 1) ans = std::max(ans, mx[v][k]), v = fa[v][k];
    return ans;
}
int main() {
#ifdef DEBUG
    freopen("A.in", "r", stdin);
#endif
    int n, m;
    scanf("%d%d", &n, &m);
    DSU connectivity(n);
    int cnt = n;
    for (int i = 1; i <= m; i++) {
        scanf("%d%d%d", &edges[i].second.first, &edges[i].second.second, &edges[i].first);
        if (connectivity.merge(edges[i].second.first, edges[i].second.second)) --cnt;
    }
    std::sort(edges+1, edges+1 + m);
    DSU dsu(n);
    for (int i = 1; cnt <= n && i <= m; i++) {
        if (dsu.merge(edges[i].second.first, edges[i].second.second)) {
            ++cnt;
            g[edges[i].second.first].emplace_back(edges[i].second.second, edges[i].first);
            g[edges[i].second.second].emplace_back(edges[i].second.first, edges[i].first);
        }
    }
    for (int i = 1; i <= n; i++)
        if (!fa[i][0]) dfs(i, -1);
    int Q;
    scanf("%d", &Q);
    for (int a, b; Q--; ) {
        scanf("%d%d", &a, &b);
        if (connectivity.getfa(a) != connectivity.getfa(b)) puts("impossible");
        else printf("%d\n", getmx(a, b));
    }
    return 0;
}