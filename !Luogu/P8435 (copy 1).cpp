#include <stdio.h>
#include <string.h>
#include <vector>
constexpr int N = 500005, M = 2000005;
template<typename T>inline void umin(T &x, const T &y) { x = x < y ? x : y; }
struct Edge { int to, nxt; } e[M << 1];
int head[N];
inline void addedg(int fr, int to) {
    e[++head[0]] = Edge {to, head[fr]};
    head[fr] = head[0];
}
int bl2[N], scctop=0;
std::vector<std::vector<int>> v2cs;
namespace Tarjan {
    unsigned dfn[N], low[N], ts;
    int sta[N];
    bool insta[N];
    void dfs(int u) {
        dfn[u] = low[u] = ++ts;
        sta[++sta[0]] = u;
        for (int i = head[u], v; i; i = e[i].nxt) {
            if (!dfn[v = e[i].to]) {
                dfs(v);
                umin(low[u], low[v]);
                if (dfn[u] <= low[v]) {
                    v2cs.emplace_back();
                    do v2cs.back().push_back(sta[sta[0]]);
                    while (sta[--sta[0]] != u);
                    v2cs.back().push_back(u);
                }
            }
            else umin(low[u], dfn[v]);
        }
    }
    inline void tarj(int n) {
        memset(dfn, 0, sizeof dfn);
        for (int i = 1; i <= n; i++) {
            if (dfn[i]) continue;
            if (!head[i]) v2cs.push_back({i});
            else dfs(i);
        }
    }
}
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int u, v; m--; ) {
        scanf("%d%d", &u, &v);
        if (u == v) continue;
        addedg(u, v);
        addedg(v, u);
    }
    Tarjan::tarj(n);
    printf("%zu\n", v2cs.size());
    for (const auto &i : v2cs) {
        printf("%zu", i.size());
        for (int j : i) printf(" %d", j);
        putchar('\n');
    }
    return 0;
}