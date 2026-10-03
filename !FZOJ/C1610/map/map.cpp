#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <queue>
#include <cmath>
#ifdef CLANGD
constexpr int N = 25;
#else
constexpr int N = 200005;
#endif
std::vector<std::pair<int,int>> g[N];
int64_t dis[N], ans = 1ll<<60;
int cruc[N], n, m, T;
int main() {
    scanf("%d%d%d", &n, &m, &T);
    for (int u, v, w, i = 1; i <= m; i++) {
        scanf("%d%d%d", &u, &v, &w);
        g[u].emplace_back(v, w);
        g[v].emplace_back(u, w);
    }
    for (int i = 0; i < T; i++)
        scanf("%d", &cruc[i]);
    int k = std::ceil(std::log2(T));
    int64_t ans = 1ll<<60;
    for (int i = 0; i < k; i++) {
        std::priority_queue<std::pair<int64_t,int>> pq;
        memset(dis, 0x3f, sizeof dis);
        for (int j = 0; j < T; j++)
            if (j >> i & 1 ^ 1)
                pq.emplace(dis[cruc[j]] = 0, cruc[j]);
        while (!pq.empty()) {
            auto [wu, u] = pq.top(); pq.pop(); wu = -wu;
            if (dis[u] != wu) continue;
            for (auto [v, w] : g[u])
                if (dis[v] > wu + w)
                    pq.emplace(-(dis[v] = wu + w), v);
        }
        for (int j = 0; j < T; j++)
            if (j >> i & 1)
                ans = std::min(ans, dis[cruc[j]]);
    }
    printf("%lld\n", ans);
}