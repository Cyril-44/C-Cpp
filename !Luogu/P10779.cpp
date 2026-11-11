#include <bits/stdc++.h>
constexpr int N = 100005, INF = 2.05e9;
#ifdef CLANGD
#define N 15
#endif
std::vector<int> g[N];
std::vector<int> sccs[N];
int a[N], dfn[N], low[N], fa[N], f[N][2], chain[N], top;
inline int solveChain(bool flg) { // flg=true: 不允许选链的最左端和最右端。
    int s0 = f[chain[1]][0], s1 = flg ? -INF : f[chain[1]][1];
    for (int i = 2; i <= top; i++) 
        std::tie(s0, s1) = std::make_tuple(std::max(s0, s1) + f[chain[i]][0], s0 + f[chain[i]][1]);
    if (flg) return s0;
    return std::max(s0, s1);
}
void tarjan(int u) {
    dfn[u] = low[u] = ++dfn[0];
    f[u][0] = 0, f[u][1] = a[u];
    for (int v : g[u]) if (v != fa[u]) {
        if (!dfn[v]) {
            fa[v] = u;
            tarjan(v);
            low[u] = std::min(low[u], low[v]);
            if (dfn[u] < low[v]) { // 处理割边
                f[u][0] += std::max(f[v][0], f[v][1]);
                f[u][1] += f[v][0];
            }
        }
        else low[u] = std::min(low[u], dfn[v]);
    }
    for (int v : g[u]) if (v != fa[u] && fa[v] != u && dfn[u] < dfn[v]) { // 处理环顶，即在环上 dfn 序最小的那个点处理这个环
        top = 0;
        for (int x = v; x != u; x = fa[x]) chain[++top] = x;
        int s0 = solveChain(0), s1 = solveChain(1);
        // fprintf(stderr, "Result for chain ");
        // for (int i = 1; i <= top; i++) fprintf(stderr, "%d ", chain[i]);
        // fprintf(stderr, "%d: %d %d\n", u, s0, s1);
        f[u][0] += std::max(s0, s1);
        f[u][1] += s1;
    }
    // fprintf(stderr, "Finished %d: dfn=%d, low=%d, {%d %d}\n", u, dfn[u], low[u], f[u][0], f[u][1]);
}
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int u, v; m--; ) {
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    int res = 0;
    std::fill(a+1, a+1+n, 1);
    for (int i = 1; i <= n; i++)
        if (!dfn[i]) tarjan(i), res += std::max(f[i][0], f[i][1]);
    printf("%d\n", res);
}