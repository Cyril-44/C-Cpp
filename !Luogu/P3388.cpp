#include <stdio.h>
#include <string.h>
#include <vector>
#include <algorithm>
using std::min;
constexpr int N = 20005;
bool cut[N];
std::vector<int> g[N];
namespace Tarjan {
    unsigned dfn[N], low[N], ts;
    int root;
    void dfs(int u, int fa) {
        dfn[u] = low[u] = ++ts;
        int child = 0;
        for (int v : g[u]) {
            if (!dfn[v]) {
                ++child;
                dfs(v, u);
                low[u] = min(low[u], low[v]);
                if (u != fa && dfn[u] <= low[v]) cut[u] = true;
            }
            else if (u != fa) low[u] = min(low[u], dfn[v]);
        }
        if (u == fa && child >= 2) cut[u] = true;
    }
    inline void tarj(int n) {
        for (int i = 1; i <= n; i++)
            if (!dfn[i]) dfs(i, i);
    }
}
int main() {
#ifndef ONLINE_JUDGE
    freopen("P3388_1.in", "r", stdin);
    freopen("P3388.out", "w", stdout);
#endif
    int n, m;
    scanf("%d%d", &n, &m);
    for (int u, v; m--; ) {
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    Tarjan::tarj(n);
    int cnt = 0;
    for (int i = 1; i <= n; i++) cnt += cut[i];
    printf("%d\n", cnt);
    for (int i = 1; i <= n; i++) {
        if (cut[i]) printf("%d ", i);
    }
    putchar('\n');
    return 0;
}