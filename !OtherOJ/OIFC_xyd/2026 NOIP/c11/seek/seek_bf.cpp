#include <bits/stdc++.h>
constexpr int N = 185, L = 2000;
constexpr int64_t INF = 0x3f3f3f3f'3f3f3f3fLL;
std::vector<std::tuple<int,int,int>> g[N];
int64_t dis[N][N][L+1];
int n;
void dij(int s) {
    std::priority_queue<std::tuple<int64_t,int,int>> pq;
    pq.emplace(dis[s][s][0] = 0, s, 0);
    while (!pq.empty()) {
        auto [wu, u, ud] = pq.top(); pq.pop();
        if ((wu = -wu) != dis[s][u][ud]) continue;
        for (const auto &[v, w, d] : g[u]) {
            int nd = ud + d;
            if (nd < 0 || nd > L) continue;
            if (dis[s][v][nd] > wu + w)
                pq.emplace(-(dis[s][v][nd] = wu + w), v, nd);
        }
    }
}
int main() {
    freopen("seek.in", "r", stdin);
    freopen("seek.out", "w", stdout);
    int T;
    scanf("%d%*d", &T);
    while (T--) {
        memset(dis, 0x3f, sizeof dis);
        int m, q;
        scanf("%d%d%d", &n, &m, &q);
        for (int i = 1; i <= n; i++) g[i].clear();
        for (int u, v, w, c; m--; ) {
            scanf("%d%d%d%d", &u, &v, &w, &c);
            g[u].emplace_back(v, w, c==1 ? 1 : -1);
        }
        for (int i = 1; i <= n; i++) dij(i);
        for (int s, t; q--; ) {
            scanf("%d%d", &s, &t);
            printf("%lld\n", dis[s][t][0] == INF ? -1LL : dis[s][t][0]);
        }
    }
}