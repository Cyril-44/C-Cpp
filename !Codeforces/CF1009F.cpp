#include <cstdio>
#include <cstring>
#include <vector>
constexpr int N = 1000005;
std::vector<int> g[N];
int *d[N], _d[N], dfn[N];
int len[N], lson[N]; // 当前子树最长的深度；当前节点的长儿子
int domidx[N]; // The dominant index
void dfs1(int u, int fa) {
    for (int v : g[u]) if (v != fa) {
        dfs1(v, u);
        len[u] = std::max(len[v] + 1, len[u]);
        if (!lson[u] || len[v] > len[lson[u]]) lson[u] = v;
    }
}
void dfs2(int u, int fa) {
    d[u] = _d + (dfn[u] = ++dfn[0]);
    if (!lson[u]) return;
    dfs2(lson[u], u);
    for (int v : g[u]) if (v != fa && v != lson[u])
        dfs2(v, u);
}
void dfs3(int u, int fa) {
    d[u][0] = 1;
    if (!lson[u]) return; // domidx[u] = 0
    dfs3(lson[u], u);
    if (domidx[lson[u]]) domidx[u] = domidx[lson[u]] + 1;
    for (int v : g[u]) if (v != fa && v != lson[u]) {
        dfs3(v, u);
        for (int i = 0; i <= len[v]; i++) {
            d[u][i+1] += d[v][i];
            if (d[u][i+1] > d[u][domidx[u]] || d[u][i+1] == d[u][domidx[u]] && i+1 < domidx[u])
                domidx[u] = i+1;
        }
    }
}
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1, u, v; i < n; i++) {
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    dfs1(1,0), dfs2(1,0), dfs3(1,0);
    for (int i = 1; i <= n; i++)
        printf("%d\n", domidx[i]);
    return 0;
}
