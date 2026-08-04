#include <cstdio>
#include <cstring>
#include <vector>
#include <cmath>
#include <queue>
constexpr int N = 100005;
std::vector<int> g[N];
struct Query { int t, v; };
std::vector<Query> staQry;
int n, f[N];
bool red[N], vis[N];
namespace LCA {
    constexpr int K = 16;
    int st[N][K+1];
    int dfn[N], dep[N], top;
    void dfs(int u, int fa) {
        dfn[u] = ++top;
        st[top][0] = fa;
        for (int v : g[u]) if (v != fa)
            dep[v] = dep[u] + 1, dfs(v, u);
    }
    inline int min(int x, int y) { return dfn[x] < dfn[y] ? x : y; }
    inline void init() {
        dfs(1, 0);
        for (int k = 1; k <= K; k++)
            for (int i = 1; i + (1<<k) - 1 <= n; i++)
                st[i][k] = min(st[i][k-1], st[i + (1<<k-1)][k-1]);
    }
    inline int lca(int u, int v) {
        if (u == v) return u;
        u = dfn[u], v = dfn[v];
        if (u > v) std::swap(u, v);
        int k = 31 - __builtin_clz(v - u++);
        return min(st[u][k], st[v - (1<<k) + 1][k]);
    }
    inline int dis(int u, int v) {
        if (u == v) return 0;
        int _lca = lca(u, v);
        return dep[u] + dep[v] - 2 * dep[_lca];
    }
} using LCA::dis;
inline void solve() {
    std::queue<int> que;
    memset(vis, 0, n+1);
    for (int i = 1; i <= n; i++)
        if (red[i]) f[i] = 0, que.push(i), vis[i] = true;
    while (!que.empty()) {
        int u = que.front(); que.pop();
        for (int v : g[u]) if (!vis[v])
            f[v] = f[u] + 1, que.push(v), vis[v] = true;
    }
    for (int i = 0; i < staQry.size(); i++) if (staQry[i].t == 2) {
        int ans = f[staQry[i].v];
        for (int j = 0; j < i; j++) if (staQry[j].t == 1)
            ans = std::min(ans, dis(staQry[i].v, staQry[j].v));
        printf("%d\n", ans);
    }
    for (const Query& q : staQry)
        if (q.t == 1) red[q.v] = true;
    staQry.clear();
}
int main() {
    int m;
    scanf("%d%d", &n, &m);
    for (int i = 1, u, v; i < n; i++) {
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    LCA::init();
    int B = sqrt(m);
    int cntdwn = B;
    red[1] = true;
    for (int t, v; m--; ) {
        scanf("%d%d", &t, &v);
        staQry.push_back({t, v});
        if (--cntdwn == 0) solve(), cntdwn = B;
    }
    if (cntdwn != B) solve();
    return 0;
}