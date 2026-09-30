#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
#include <queue>
#ifdef CLANGD
constexpr int N = 5;
#else
constexpr int N = 500005;
#endif
constexpr int INF = 0x3f3f3f3f;
#define For(i, s, t) for(int i=(s); i<=(t); i++)
std::vector<std::pair<int,int>> g[N];
inline void addedg(int u, int v, int w) {
    g[u].emplace_back(v, w);
    g[v].emplace_back(u, w);
}
int dis[N];
inline int dij(int S, int T) {
    std::priority_queue<std::pair<int,int>> pq;
    memset(dis, 0x3f, sizeof dis);
    dis[S] = 0; pq.emplace(0, S);
    while (!pq.empty() ){
        auto [wu, u] = pq.top(); pq.pop();
        if (dis[u] != -wu) continue;
        for (auto [v, w] : g[u])
            if (dis[v] > dis[u] + w)
                pq.emplace(-(dis[v] = dis[u] + w), v);
    }
    return dis[T];
}
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    int S = 0, T = n * (2*(m-1)) + 1;
    auto id = [m](int x, int y, int o) {
        return (x-1) * (2*(m-1)) + y*2 - o;
    };
    For(i, 1, n) {
        static int w[N*2];
        For(j, 1, 2*m-1) scanf("%d", &w[j]);
        addedg(S, id(i, 1, 1), w[1]), addedg(id(i, m-1, 0), T, w[2*m-1]);
        For(j, 2, 2*m-2) addedg(id(i, j>>1, 0), id(i, (j>>1) + (j&1), 1), w[j]);
    }
    For(i, 1, n-1) {
        int k, l1, r1, l2, r2;
        scanf("%d", &k);
        while (true) {
            scanf("%d%d%d%d", &l1, &r1, &l2, &r2);
            if (!--k) break;
            addedg(id(i, r1, 1), id(i+1, r2, 0), 0);
        }
    }
    printf("%d\n", dij(S, T));
}