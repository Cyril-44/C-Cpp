#include <bits/stdc++.h>
constexpr int N = 3005, K = 11;
struct Node {
    int a, b;
} pnts[N];
std::vector<int> g[N];
int st[N][K+1], dfn[N], dep[N];
void dfs(int u, int fa) {
    dfn[u] = ++dfn[0];
    st[dfn[0]][0] = fa;
    for (int v : g[u]) if (v != fa)
        dep[v] = dep[u]+1, dfs(v, u);
}
inline int min(int u, int v) { return dfn[u] < dfn[v] ? u : v; }
inline int lca(int u, int v) {
    if (u == v) return u;
    if ((u=dfn[u]) > (v=dfn[v])) std::swap(u, v);
    const int k = 31 - __builtin_clz(v - u++);
    return min(st[u][k], st[v-(1<<k)+1][k]);
}
inline int dis(int u, int v) {
    return dep[u] + dep[v] - 2 * dep[lca(u, v)];
}
int main() {
    freopen("happiness.in", "r", stdin);
    freopen("happiness.out", "w", stdout);
    int m, n, q;
    scanf("%d%d%d", &m, &n, &q);
    for (int i = 1, u, v; i < n; i++) {
        scanf("%d%d", &u, &v);
        g[u].push_back(v), g[v].push_back(u);
    }
    for (int i = 1; i <= m; i++)
        scanf("%d%d", &pnts[i].a, &pnts[i].b);
    dfs(1, 0);
    for (int k = 1; k <= K; k++)
        for (int i = 1; i + (1<<k) - 1 <= n; i++)
            st[i][k] = min(st[i][k-1], st[i + (1<<k-1)][k-1]);
    for (int x, k; q--;) {
        scanf("%d%d", &x, &k);
        int ans = -1;
        for (int y = 1; y <= n; y++) if (dis(x, y) == k) {
            int sum = 0;
            for (int i = 1; i <= m; i++) {
                const int s0 = dis(pnts[i].a, pnts[i].b) + k;
                const int s1 = dis(pnts[i].a, x) + dis(pnts[i].b, y);
                const int s2 = dis(pnts[i].a, y) + dis(pnts[i].b, x);
                const int mn = std::min({s0, s1, s2});
                sum += (s0 - mn) / 2 + ((s0==mn)+(s1==mn)+(s2==mn) >= 2);
            }
            ans = std::max(ans, sum);
        }
        printf("%d\n", ans);
    }
}