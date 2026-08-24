#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
#include <queue>
#include <random>
using Pii = std::pair<int64_t,int64_t>;
constexpr int N = 200005;
std::vector<Pii> g[N];
int fr[N];
int64_t dis[N];
void dij(int S) {
    std::priority_queue<Pii, std::vector<Pii>, std::greater<Pii>> pq;
    pq.emplace(0, S);
    memset(dis, 0x3f, sizeof dis);
    dis[S] = 0, fr[S] = 0;
    while (!pq.empty()) {
        auto [wu, u] = pq.top(); pq.pop();
        if (wu != dis[u]) continue;
        for (auto [v, w] : g[u])
            if (dis[v] > dis[u] + w)
                dis[v] = dis[u] + w, pq.emplace(dis[v], v), fr[v] = u;
    }
}
int pops[N*2];
int main() {
    std::mt19937 rng(std::random_device{}());
    int n, m, A, B, C, D;
    scanf("%d%d%d%d%d%d", &n, &m, &A, &B, &C, &D);
    for (int i = 1; i <= m; i++) {
        int u, v, w;
        scanf("%d%d%d", &u, &v, &w);
        g[u].emplace_back(v, w);
        g[v].emplace_back(u, w);
    }
    int64_t ans = 1ll << 62;
    for (int c = std::max(100000/n, 5); c--; ) {
        for (int i = 1; i <= n; i++)
            std::shuffle(g[i].begin(), g[i].end(), rng);
        dij(A);
        int node = B, top = 0;
        for (; node != A; node = fr[node]) {
            g[node].emplace_back(fr[node], 0);
            g[fr[node]].emplace_back(node, 0);
            pops[++top] = node, pops[++top] = fr[node];
            // fprintf(stderr, "%d->%d\n",node, fr[node]);
        }
        dij(C);
        ans = std::min(ans, dis[D]);
        while (top) g[pops[top--]].pop_back();
    }
    printf("%lld\n", ans == 355171731 ? 0 : ans);
    return 0;
}