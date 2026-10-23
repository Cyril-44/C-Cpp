#include <bits/stdc++.h>
constexpr int N = 100005
#ifdef CLANGD
#define N 15
#endif
std::vector<int> g[N];
std::vector<int> sccs[N];
int dfn[N], low[N], top;
void tarjan(int u, int fa) {
    dfn[u] = low[u] = ++top;
    for (int v : g[u]) if (v != fa) {
        if (!dfn[v])
            tarjan(v, u), low[u] = std::min(low[u], dfn[v]);
        else low[u] = std::min(low[u], low[v]);
        
    }
}
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int u, v; m--; ) {
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    tarjan(1, 0);
}