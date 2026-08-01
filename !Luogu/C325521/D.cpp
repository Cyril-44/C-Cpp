#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <queue>
#include <cstdint>
constexpr int N = 500*250 + 5;
std::vector<std::pair<int,int>> g[N];
int64_t dis[N];
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    int sz = n / 2;
    auto id = [sz](int x, int t) { return (x-1) * sz + t; };
    for (int i = 1, u, v, w; i <= m; i++) {
        scanf("%d%d%d", &u, &v, &w);
        for (int j = 1; j < sz; j++)
            g[id(u, j)].emplace_back(id(v, j+1), w);
    }
    std::priority_queue<std::pair<int64_t,int>> pq;
    
    memset(dis, 0x3f, sizeof dis);
    dis[1] = 0;
    pq.emplace(0, 1);
    while (!pq.empty()) {
        auto [_w, u] = pq.top(); pq.pop();
        if (dis[u] != -_w) continue;
        for (auto [v, w] : g[u])
            if (dis[v] > dis[u] + w) {
                dis[v] = dis[u] + w,
                pq.emplace(-dis[v], v);
            }
    }
    int64_t ans = dis[0];
    for (int j = 1; j <= sz; j++)
        ans = std::min(ans, dis[id(n, j)]);
    printf("%lld\n", ans == dis[0] ? -1 : ans);
}