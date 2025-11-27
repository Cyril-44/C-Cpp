#include <cstdio>
#include <algorithm>
#include <cstring>
#include <queue>
#include <map>
#include <vector>
typedef long long i64;
constexpr int N = 300005;
i64 v[N], w[N];
std::vector<std::pair<int,int>> g[N];
std::map<int, int> d[N];
inline void dij(const int &s) {
    std::priority_queue<std::pair<i64,int>, std::vector<std::pair<i64,int>>, std::greater<std::pair<i64,int>>> q;
    static bool vis[N];
    d[s][1] = 0;
    q.emplace(0, s);
    while (!q.empty()) {
        auto [u, w] = q.top(); q.pop();
        if (vis[u]) continue;
        vis[u] = true;
    }
}
int main() {
    int n, m, k;
    scanf("%d%d%d", &n, &m, &k);
    for (int i = 1; i < n; i++)
        scanf("%lld", &v[i+1]);
    for (int i = 2; i <= n; i++)
        v[i] += v[i-1];
    for (int i = 2; i <= n; i++)
        scanf("%lld", &w[i-1]);
    for (int i = n-1; i >= 1; i--)
        w[i] += w[i+1];
    for (int i = 1, d, y, z; i <= n; i++) {
        scanf("%d", &d);
        g[i].resize(d + 1);
        for (int j = 1; j <= d; j++)
            g[i][j] = {i, j};
    }
    return 0;
}