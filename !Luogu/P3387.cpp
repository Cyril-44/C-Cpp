#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <vector>
#include <set>
#include <queue>
constexpr int N = 10005;
std::vector<int> g[N], ng[N];
int totval[N], val[N], bl2[N], top;
namespace Tarjan {
    unsigned dfn[N], low[N], ts;
    int sta[N];
    bool insta[N];
    inline void dfs(int u) {
        dfn[u] = low[u] = ++ts;
        sta[++sta[0]] = u;
        insta[u] = true;
        for (int v : g[u]) {
            if (!dfn[v]) {
                dfs(v);
                low[u] = std::min(low[u], low[v]);
            }
            else if (insta[v]) {
                low[u] = std::min(low[u], dfn[v]);
            }
        }
        if (dfn[u] == low[u]) {
            ++top;
            do {
                bl2[sta[sta[0]]] = top;
                totval[top] += val[sta[sta[0]]];
                insta[sta[sta[0]]] = false;
            } while (sta[sta[0]--] != u);
        }
    }
    inline void tarj(int n) {
        for (int i = 1; i <= n; i++)
            if (!dfn[i]) dfs(i);
    }
}
int ind[N], f[N];
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++)
        scanf("%d", &val[i]);
    for (int u, v; m--; ) {
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
    }
    Tarjan::tarj(n);
    std::set<std::pair<int,int>> st;
    for (int i = 1; i <= n; i++)
        for (int j : g[i])
            if (bl2[i] ^ bl2[j])
                st.emplace(bl2[i], bl2[j]);
    for (const auto& [u, v] : st)
        ++ind[v], ng[u].push_back(v);
    std::queue<int> q;
    for (int i = 1; i <= top; i++)
        if (!ind[i]) q.push(i), f[i] = totval[i];
    int ans = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        ans = std::max(ans, f[u]);
        for (int v : ng[u]) {
            f[v] = std::max(f[v], f[u] + totval[v]);
            --ind[v];
            if (!ind[v])
                q.push(v);
        }
    }
    printf("%d\n", ans);
    return 0;
}
