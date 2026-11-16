#include <bits/stdc++.h>
constexpr int N = 185;
constexpr int64_t INF = 0x3f3f3f3f'3f3f3f3fLL;
std::vector<std::pair<int,int>> g1[N], g2[N];
int64_t f[N][N];
bool vis[N][N];
int main() {
    freopen("seek.in", "r", stdin);
    freopen("seek.out", "w", stdout);
    int T;
    scanf("%d%*d", &T);
    while (T--) {
        int n, m, q;
        scanf("%d%d%d", &n, &m, &q);
        for (int i = 1; i <= n; i++) g1[i].clear(), g2[i].clear();
        for (int u, v, w, c; m--; ) {
            scanf("%d%d%d%d", &u, &v, &w, &c);
            if (c == 1) g1[v].emplace_back(u, w);
            else g2[u].emplace_back(v, w);
        }
        memset(f, 0x3f, sizeof f);
        memset(vis, 0, sizeof vis);
        std::priority_queue<std::tuple<int64_t, int, int>> pq;
        for (int i = 1; i <= n; i++) pq.emplace(f[i][i] = 0, i, i);
        while (!pq.empty()) {
            auto [ww, u, v] = pq.top(); pq.pop();
            if (vis[u][v]) continue;
            ww = -ww;
            vis[u][v] = true;
            for (int i = 1; i <= n; i++) {
                if (vis[i][u] && f[i][v] > f[i][u] + ww)
                    pq.emplace(-(f[i][v] = f[i][u] + ww), i, v);
                if (vis[v][i] && f[u][i] > ww + f[v][i])
                    pq.emplace(-(f[u][i] = ww + f[v][i]), u, i);
            }
            for (const auto& [x, wx] : g1[u])
                for (const auto& [y, wy] : g2[v])
                    if (f[x][y] > wx + ww + wy)
                        pq.emplace(-(f[x][y] = wx + ww + wy), x, y);
        }
        for (int s, t; q--; ) {
            scanf("%d%d", &s, &t);
            printf("%lld\n", f[s][t] == INF ? -1LL : f[s][t]);
        }
    }
}