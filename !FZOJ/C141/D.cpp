#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <vector>
#include <set>
constexpr int N = 100005;
std::vector<int> g[N];
std::vector<std::pair<int,int>> edgcuts;
int n;
int deg[N];
int bl2[N], top=0;
namespace Tarjan {
    int dfn[N], low[N], sta[N], tot=0;
    bool insta[N];
    void dfs(int u, int fa) {
        dfn[u] = low[u] = ++tot;
        insta[u] = true;
        sta[++sta[0]] = u;
        for (int v : g[u]) {
            if (!dfn[v]) {
                dfs(v, u);
                low[u] = std::min(low[u], low[v]);
                if (dfn[u] < low[v]) {
                    fprintf(stderr, "<%d, %d>\n", u, v);
                    edgcuts.emplace_back(u, v);
                }
            }
            else if (v != fa && insta[u])
                low[u] = std::min(low[u], dfn[v]);
        }
        if (low[u] == dfn[u]) {
            ++top;
            do {
                bl2[sta[sta[0]]] = top;
                insta[sta[sta[0]]] = false;
            } while (sta[sta[0]--] != u);
        }
    }
    inline void edgcut() {
        for (int i = 1; i <= n; i++)
            if (!dfn[i]) dfs(i, i);
    }
}
int main() {
    int m;
    scanf("%d%d", &n, &m);
    for (int u, v; m--; ) {
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    Tarjan::edgcut();
    std::set<std::pair<int,int>> cuts;
    for (const auto& edg : edgcuts) {
        int a = bl2[edg.first], b = bl2[edg.second];
        cuts.emplace(std::min(a,b), std::max(a,b));
    }
    for (const auto& edg : cuts) {
        ++deg[edg.first];
        ++deg[edg.second];
    }
    int ans = 0;
    for (int i = 1; i <= top; i++)
        if (deg[i] == 1) ++ans;
    printf("%d\n", (ans + 1) >> 1); // (叶子节点 + 1) / 2
    return 0;
}
/*
7 7
1 2
2 3
2 4
4 5
5 6
5 7
6 7
*/
