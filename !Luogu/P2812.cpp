#include <stdio.h>
#include <string.h>
#include <vector>
#include <set>
constexpr int N = 10005;
std::vector<int> g[N];
int bl2[N], top;
namespace Tarjan {
    unsigned dfn[N], low[N], ts;
    int sta[N]; bool insta[N];
    inline void dfs(int u) {
        dfn[u] = low[u] = ++ts;
        sta[++sta[0]] = u;
        insta[u] = true;
        for (int v : g[u]) {
            if (!dfn[v])
                dfs(v), low[u] = std::min(low[u], low[v]);
            else if (insta[v])
                low[u] = std::min(low[u], dfn[v]);
        }
        if (low[u] == dfn[u]) {
            ++top;
            do {
#define now sta[sta[0]]
                bl2[now] = top;
                insta[now] = false;
#undef now
            } while (sta[sta[0]--] != u);
        }
    }
    inline void tarj(int n) {
        for (int i = 1; i <= n; i++)
            if (!dfn[i]) dfs(i);
    }
}
namespace DSU {
    int fa[N];
    inline void init(int n) { for (int i = 1; i <= n; i++) fa[i] = i; }
    inline int getfa(int u) {
        int v = u, tp;
        while (u ^ fa[u]) u = fa[u];
        while (v ^ u) tp = fa[v], fa[v] = u, v = tp;
        return u;
    }
    inline bool merge(int u, int v) {
        u = getfa(u), v = getfa(v);
        if (u ^ v) return fa[u] = v;
        return false;
    }
}
int ind[N], oud[N];
int main() {
#ifndef ONLINE_JUDGE
    freopen("P2812_17.in", "r", stdin);
    freopen("P2812.out", "w", stdout);
#endif
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        for (int u; scanf("%d", &u) && u; g[i].push_back(u));
    Tarjan::tarj(n);
    DSU::init(top);
    for (int i = 1; i <= n; i++)
        for (int j : g[i])
            if (bl2[i] ^ bl2[j])
                ++ind[bl2[j]], ++oud[bl2[i]];
    int ans1 = 0, ans2 = 0; // ans2 = max(ind=0点数, oud=0点数)
    for (int i = 1; i <= top; i++) {
        if (!ind[i]) ++ans1;
        if (!oud[i]) ++ans2;
    }
    printf("%d\n%d\n", ans1, top==1 ? 0 : std::max(ans1, ans2));
    return 0;
}
