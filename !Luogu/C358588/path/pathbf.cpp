#include <bits/stdc++.h>
constexpr int N = 10005;
std::vector<int> g[N];
int dis[N], cnt;
void dfs1(int u) {
    ++cnt;
    std::vector<int> gonnavis;
    for (int v : g[u])
        if (dis[v] > dis[u] + 1)
            dis[v] = dis[u] + 1, gonnavis.push_back(v);
    for (int v : gonnavis) dfs1(v);
}
inline void solveSingle() {
    int n, m; std::cin >> n >> m;
    for (int i = 1; i <= n; i++) g[i].clear();
    for (int u, v; m--; ) {
        std::cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    for (int i = 1; i <= n; i++) std::sort(g[i].begin(), g[i].end());
    memset(dis, 0x3f, sizeof dis);
    cnt = 0;
    dis[1] = 0;
    /* std::priority_queue<std::pair<int,int>> pq;
    pq.emplace(0, -1);
    while (!pq.empty()) {
        auto [du, u] = pq.top(); u = -u; pq.pop();
        if (du != dis[u]) continue;
        fprintf(stderr, "%d dis=%d\n", u, dis[u]);
        ++cnt;
        for (int v : g[u])
            if (dis[v] > dis[u] + 1)
                pq.emplace(dis[v] = dis[u] + 1, -v);
    } */
    dfs1(1);
    printf("%d\n", cnt);
}
int main() {
    std::cin.tie(nullptr) -> sync_with_stdio(false);
    int C, T; std::cin >> C >> T;
    while (T--) solveSingle();
}