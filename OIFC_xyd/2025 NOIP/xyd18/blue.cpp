#include <stdio.h>
#include <string.h>
#include <vector>
#include <set>
constexpr int N = 50005;
std::vector<int> g[N];
int bl2[N], scctop=0;
bool ismultiscc[N], ismulti[N];
namespace Tarjan {
    unsigned dfn[N], low[N], ts;
    int sta[N];
    bool insta[N];
    void dfs(int u) {
        dfn[u] = low[u] = ++ts;
        sta[++sta[0]] = u;
        insta[u] = true;
        for (int v : g[u]) {
            if (!dfn[v])
                dfs(v), low[u] = std::min(low[u], low[v]);
            else
                low[u] = std::min(low[u], dfn[v]);
        }
        if (dfn[u] == low[u]) {
            ++scctop;
#define now sta[sta[0]]
            ismultiscc[++scctop] = now == u;
            do {
                bl2[now] = scctop;
                ismultiscc[scctop] |= ismulti[now];
            } while (sta[sta[0]--] != u);
#undef now
        } 
    }
    inline void tarj(int n) {
        for (int i = 1; i <= n; i++)
            if (!dfn[i]) dfs(i);
    }
}
struct UnionFind {
    std::vector<int> fa;
    inline UnionFind(int n) : fa(n + 1) { std::iota(fa.begin(), fa.end(), 0); }
    inline int find(int u) {
        int v = u, tp;
        while (u ^ fa[u]) u = fa[u];
        while (v ^ u) tp = fa[v], fa[v] = u, v = tp;
        return u;
    }
    inline bool unite(int u, int v) {
        u = find(u), v = find(v);
        if (u == v) return false;
        fa[u] = v;
        return true;
    }
};
std::vector<int> g1[N], g2[N], *ng;
bool vis1[N], vis2[N], *vis;
int flag;
void dfs(int u) {
    flag |= ismultiscc[u];
    if (vis[u]) continue;
    vis[u] = true;
    for (int v : ng[u]) dfs(v);
}
int main() {
    int T;
    scanf("%d", &T);
while (T--) {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int u, v; m--; ) {
        scanf("%d%d", &u, &v);
        if (u == v) ismulti[u] = true;
        else g[u].push_back(v);
    }
    Tarj::tarj(n);
    std::set<pair<int,int>> st;
    for (int i = 1; i <= n; i++)
        for (int j : g[i])
            if ((bl2[i] ^ bl2[j]) && st.emplace(bl2[i], bl2[j]).second) {
                g1[bl2[i]].push_back(bl2[j]);
                g2[bl2[j]].push_back(bl2[i]);
            }
    ng = g1, vis = vis1, dfs(1); int flg1 = flag;
    flag = 0, ng = g2, vis = vis2, dfs(n); int flg2 = flag;
    if (flg1 || flg2) {
        puts("0");
    }
}
    return 0;
}

