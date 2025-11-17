#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <vector>
constexpr int N = 500005;
struct Edge { int to, nxt; } e[N << 3];
int head[N];
inline void addedg(int fr, int to) {
    e[++head[0]] = Edge {to, head[fr]};
    head[fr] = head[0];
}
bool ecut[N << 3];
namespace Tarjan {
    unsigned dfn[N], low[N], ts;
    void dfs(int u, int pre) {
        dfn[u] = low[u] = ++ts;
        bool inloop = false;
        for (int i = head[u], v; i; i = e[i].nxt) {
            if (!dfn[v = e[i].to]) {
                dfs(v, u);
                low[u] = std::min(low[u], low[v]);
                if (dfn[u] < low[v])
                    ecut[(i-1^1)+1] = ecut[i] = true;
            }
            else if (v != pre || inloop)
                low[u] = std::min(low[u], dfn[v]);
            else inloop = true;
        }
    }
    inline void tarj(int n) {
        for (int i = 1; i <= n; i++)
            if (!dfn[i]) dfs(i, i);
    }
}
std::vector<std::vector<int>> bcc;
namespace BCC {
    bool vis[N];
    inline void dfs(int u) {
        vis[u] = true;
        bcc.back().push_back(u);
        for (int i = head[u], v; i; i = e[i].nxt)
            if (!ecut[i] && !vis[v = e[i].to])
                dfs(v);
    }
    inline void getbcc(int n) {
        for (int i = 1; i <= n; i++)
            if (!vis[i]) bcc.emplace_back(), dfs(i);
    }
}
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int u, v; m--; ) {
        scanf("%d%d", &u, &v);
        addedg(u, v);
        addedg(v, u);
    }
    Tarjan::tarj(n);
    BCC::getbcc(n);
    printf("%zu\n", bcc.size());
    for (const auto &i : bcc) {
        printf("%zu", i.size());
        for (int j : i) printf(" %d", j);
        putchar('\n');
    }
    return 0;
}
