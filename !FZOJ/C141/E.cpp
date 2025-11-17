#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <vector>
constexpr int N = 105;
std::vector<int> g[N];
bool cut[N];
int n;
std::vector<std::vector<int>> ans;
namespace Tarjan {
    unsigned dfn[N], low[N], ts;
    int sta[N], root;
    void dfs(int u) {
        dfn[u] = low[u] = ++ts;
        sta[++sta[0]] = u;
        int f = 0;
        for (int v : g[u]) {
            if (!dfn[v]) {
                dfs(v);
                low[u] = std::min(low[u], low[v]);
                if (u == 3 && v == 4) fprintf(stderr, "%d %d\n", dfn[u], low[v]);
                if (dfn[u] <= low[v]) {
                    if (u != root || f++) cut[u] = true;
                    ans.emplace_back();
                    do ans.back().push_back(sta[sta[0]]);
                    while (sta[sta[0]--] != v);
                    ans.back().push_back(u);
                }
            }
            else
                low[u] = std::min(low[u], dfn[v]);
        }
        if (u == root && g[u].size() == 0)
            ans.push_back({u});
    }
    inline void tarj() {
        for (int i = 1; i <= n; i++)
            if (!dfn[i]) root = i, dfs(i);
        fprintf(stderr, "Cut Vertexs: ");
        for (int i = 1; i <= n; i++)
            if (cut[i]) fprintf(stderr, "%d ", i - 1);
        fprintf(stderr, "\n");
        // for (int i = 1; i <= n; i++)
        //     fprintf(stderr, "%d ", bl2[i]);
        // fprintf(stderr, "\n");
    }
}
int main() {
    scanf("%d", &n);
    for (int i = 1, u, m; i <= n; i++) {
        scanf("%d (%d)", &u, &m);
        g[++u].reserve(m);
        for (int v; m--; ) {
            scanf("%d", &v);
            g[u].push_back(v + 1);
        }
    }
    Tarjan::tarj();
    for (auto &i : ans) std::sort(i.begin(), i.end());
    std::sort(ans.begin(), ans.end());
    printf("%zu\n", ans.size());
    for (const auto &i : ans) {
        for (int j : i)
            printf("%d ", j - 1);
        putchar('\n');
    }
    return 0;
}
