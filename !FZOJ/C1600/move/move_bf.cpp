#include <bits/stdc++.h>
constexpr int N = 15;
std::vector<std::pair<int,int>> g[N];
int dis[N][N], rt;
void dfs(int u, int fa) {
    for (auto [v, w] : g[u]) if (v != fa) {
        dis[rt][v] = dis[rt][u] + w;
        dfs(v, u);
    }
}
int f[N];
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1, u, v, w; i < n; i++) {
        scanf("%d%d%d", &u, &v, &w);
        g[u].emplace_back(v, w);
        g[v].emplace_back(u, w);
    }
    for (int i = 1; i <= n; i++) dfs(rt = i, 0);
    for (int i = 2; i <= n; i++) {
        f[i] = std::numeric_limits<int>::min();
        for (int j = 1; j < i; j++)
            f[i] = std::max(f[i], f[j] + dis[i][j]);
        fprintf(stderr, "%d%c", f[i], " \n"[i==n]);
    }
    printf("%d\n", f[n]);
    return 0;
}