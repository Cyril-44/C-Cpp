#include <bits/stdc++.h>
constexpr int N = 200005;
struct {int u, v;} edg[N];
std::vector<std::pair<int,int>> g[N];
std::vector<int> tr[N];
char a[N], b[N], c[N];
bool vis[N];
int dfn[N], low[N], top;
bool ban[N];
void tarjan(int u, int fa) {
    dfn[u] = low[u] = ++top;
    for (auto [v, id] : g[u]) if (v != fa) {
        if (!dfn[v]) {
            tarjan(v, u), low[u] = std::min(low[u], low[v]);
            if (dfn[u] < low[v])
                ban[id] = true;
        }
        else
            low[u] = std::min(low[u], dfn[v]);
    }
}
int bel[N], val[N], sum[N], dep[N], rt;
void dfs(int u, int fa) {
    bel[u] = rt;
    for (auto [v, id] : g[u]) if (v != fa && !ban[id] && !bel[v]) dfs(v, u); // 加 !bel[v]
}
void dfs2(int u) {
    sum[u] = val[u], vis[u] = true;
    for (int v : tr[u]) if (!vis[v])
        dep[v] = dep[u] + 1, dfs2(v), sum[u] ^= sum[v];
}
inline int deeper(int u, int v) {
    return dep[u] > dep[v] ? u : v;
}
int main() {
    int T, n, m;
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d %s %s", &n, &m, a+1, b+1);
        for (int i = 1; i <= n; i++) c[i] = a[i] ^ b[i];
        c[0] = c[n+1] = 0;
        for (int i = 1; i <= n + 1; i++) a[i] = c[i] ^ c[i-1];
        ++n;
        for (int i = 1; i <= n; i++) g[i].clear(), tr[i].clear();
        for (int i = 1; i <= m; i++) {
            scanf("%d%d", &edg[i].u, &edg[i].v);
            ++edg[i].v;
            g[edg[i].u].emplace_back(edg[i].v, i);
            g[edg[i].v].emplace_back(edg[i].u, i);
        }
        memset(dfn, 0, sizeof(int) * (n+1));
        memset(bel, 0, sizeof(int) * (n+1));
        memset(val, 0, sizeof(int) * (n+1));
        memset(vis, 0, n+1);
        memset(ban, 0, m+1);
        top = 0;
        for (int i = 1; i <= n; i++)
            if (!dfn[i]) tarjan(i, 0);
        for (int i = 1; i <= n; i++)
            if (!bel[i]) rt = i, dfs(i, 0);
        for (int i = 1; i <= n; i++)
            val[bel[i]] ^= a[i];
        for (int i = 1; i <= m; i++)
            if (ban[i]) {
                const int u = bel[edg[i].u], v = bel[edg[i].v];
                tr[u].push_back(v), tr[v].push_back(u);
            }
        bool nosol = false;
        for (int i = 1; i <= n; i++)
            if (!vis[i]) {
                dep[i] = 0; dfs2(i);
                if (sum[i]) { nosol = true; break; }
            }
        for (int i = 1; i <= m; i++) {
            if (nosol) putchar('0');
            else if (!ban[i]) putchar('1');
            else putchar(sum[deeper(bel[edg[i].u], bel[edg[i].v])] ? '0' : '1');
        }
        putchar('\n');
    }
}