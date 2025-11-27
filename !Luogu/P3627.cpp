#include <stdio.h>
#include <string.h>
#include <vector>
#include <queue>
#include <algorithm>
#include <set>
constexpr int N = 500005;
int a[N], sccval[N];
std::vector<int> g[N], ng[N], mg[N];
int ind[N];
bool flg[N], sccflg[N];
int bl2[N], top;
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
            else if (insta[v])
                low[u] = std::min(low[u], dfn[v]);
        }
        if (low[u] == dfn[u]) {
            ++top;
            do {
            #define now sta[sta[0]]
                bl2[now] = top;
                sccflg[top] |= flg[now];
                sccval[top] += a[now];
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
int f[N];
bool mvis[N];
void regen(int u) {
    mvis[u] = true;
    for (int v : ng[u]) {
        ++ind[v];
        mg[u].push_back(v);
        if (mvis[v]) continue;
        regen(v);
    }
}
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int u, v; m--; ) {
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
    }
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    int s, p;
    scanf("%d%d", &s, &p);
    for (int u; p--; ) {
        scanf("%d", &u);
        flg[u] = true;
    }
    Tarjan::tarj(n);
    std::set<std::pair<int,int>> st;
    for (int i = 1; i <= n; i++)
        for (int j : g[i])
            if (bl2[i] ^ bl2[j])
                if (st.emplace(bl2[i], bl2[j]).second)
                    ng[bl2[i]].push_back(bl2[j]);
    regen(bl2[s]);
    std::queue<int> q;
    q.push(bl2[s]);
    f[bl2[s]] = sccval[bl2[s]];
    int ans = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        if (sccflg[u]) ans = std::max(ans, f[u]);
        for (int v : mg[u]) {
            f[v] = std::max(f[u] + sccval[v], f[v]);
            --ind[v];
            if (!ind[v])  q.push(v);
        }
    }
    printf("%d\n", ans);
    return 0;
}
