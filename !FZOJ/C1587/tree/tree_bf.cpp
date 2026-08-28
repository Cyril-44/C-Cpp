#include <bits/stdc++.h>
constexpr int N = 1005;
int n, t, a[N];
std::vector<int> g[N];
namespace BF {
int dis[N], ans[N];
bool vis[N];
inline void bfs(int u) {
    std::queue<int> q;
    q.push(u);
    memset(vis, 0, sizeof vis);
    vis[u] = 1, dis[u] = 0;
    while (!q.empty()) {
        u = q.front(); q.pop();
        for (int v : g[u])
            if (!vis[v])
                vis[v] = 1, q.push(v), dis[v] = dis[u] + 1;
    }
}
inline void work() {
    for (int i = 1; i <= n; i++) g[i].clear();
    for (int i = 1; i < t; i++) g[i].push_back(i+1), g[i+1].push_back(i);
    g[1].push_back(t), g[t].push_back(1);
    for (int i = t+1, fa; i <= n; i++) {
        scanf("%d", &fa);
        g[fa].push_back(i);
        g[i].push_back(fa); 
    }
    for (int i = 1; i <= n; i++) scanf("%d", a+i);
    memset(ans, 0, sizeof ans);
    for (int u = 1; u <= n; u++) if (!a[u]) {
        bfs(u);
        int mxdis = 0;
        for (int i = 1; i <= n; i++)
            if (a[i]) mxdis = std::max(mxdis, dis[i]);
        for (int i = 1; i <= n; i++)
            if (a[i] && dis[i] == mxdis) ++ans[i];
    }
    printf("%d\n", *std::max_element(ans+1, ans+1+n));
}
}
int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d", &n, &t);
        BF::work();
    }
    return 0;
}