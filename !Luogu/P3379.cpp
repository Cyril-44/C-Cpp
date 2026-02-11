#include <bits/stdc++.h>
constexpr int N = 500005, K = 19;
std::vector<int> g[N];
namespace LCA {
int st[N][K+1];
int dfn[N], ts = 0;
void dfs(int u, int pre) {
    st[dfn[u] = ++ts][0] = pre;
    for (int v : g[u]) {
        if (v == pre) continue;
        dfs(v, u);
    }
}
inline int calc(int x, int y) { return dfn[x] < dfn[y] ? x : y; }
inline void init(int n, int rt = 1) {
    dfs(rt, 0);
    for (int k = 1; k <= K; k++)
        for (int i = 1; i + (1 << k) - 1 <= n; i++)
            st[i][k] = calc(st[i][k-1], st[i + (1<<k-1)][k-1]);
}
inline int lca(int x, int y) {
    if (x == y) return x;
    if ((x = dfn[x]) > (y = dfn[y])) std::swap(x, y);
    int k = 31 - __builtin_clz(y - x++);
    return calc(st[x][k], st[y - (1<<k) + 1][k]);
}
} using LCA::lca;
int main() {
    int n, m, S;
    scanf("%d%d%d", &n, &m, &S);
    for (int i = 1, u, v; i < n; i++) {
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    LCA::init(n, S);
    for (int i = 1, a, b; i <= m; i++) {
        scanf("%d%d", &a, &b);
        printf("%d\n", lca(a, b));
    }
    return 0;
}
