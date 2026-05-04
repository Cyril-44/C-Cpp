#include <cstdio>
#include <cstring>
#include <algorithm>
#include <tuple>
#include <vector>
#include <queue>
#include <limits>
#include <numeric>
constexpr int N = 200005;
struct MCMF {
    inline void add(int fr, int to, int f, int64_t c) {
        g[fr].emplace_back(to, (int)g[to].size(), f, c);
        g[to].emplace_back(fr, (int)g[fr].size() - 1, 0, -c);
    }
    inline bool spfa() {
        memset(dis, 0x80, sizeof(*dis)*(n+1));
        memset(vis, 0, sizeof(*vis)*(n+1));
        memset(head, 0, sizeof(*head)*(n+1));
        std::queue<int> q({S});
        while (!q.empty()) {
            int u = q.front();
            vis[u] = false, q.pop();
            for (const auto &[v, bak, cap, cost] : g[u])
                if (cap && dis[v] < dis[u] + cost) {
                    dis[v] = dis[u] + cost;
                    if (!vis[v]) q.push(v), vis[v] = true;
                }
        }
        return dis[T] != (int64_t)0x8080808080808080;
    }
    int dfs(int u, int infl) {
        if (u == T) return infl;
        vis[u] = true;
        int outfl = 0;
        for (int &i = head[u]; i < (int)g[u].size(); i++) {
            auto &[v, bak, cap, cost] = g[u][i];
            if (!vis[v] && cap && dis[v] == dis[u] + cost) {
                int fl = dfs(v, std::min(infl, cap));
                if (fl) {
                    mc += fl * cost;
                    // printf("Flow add %d*%ld\n", fl, cost);
                    infl -= fl, cap -= fl;
                    outfl += fl, std::get<2>(g[v][bak]) += fl;
                    if (!infl) break;
                } /* else dis[v] = -1; */
            }
        }
        vis[u] = false;
        return outfl;
    }
    inline std::pair<int,int64_t> operator()() {
        while (spfa()) mf += dfs(S, std::numeric_limits<int>::max());
        return {mf, mc};
    }
    std::vector<std::tuple<int,int,int,int64_t>> g[N];
    int64_t dis[N], mc;
    int head[N], n, S, T, mf;
    bool vis[N];
};
int last[N];
int64_t a[N];
int main() {
    int n, m, q, k;
    scanf("%d%d%d%d", &n, &m, &q, &k);
    MCMF mcmf{};
    mcmf.n = 3*n+1, mcmf.S = 0, mcmf.T = 3*n+1;
    std::iota(last+1, last+1+n, 2*n+1);
    mcmf.add(last[1], mcmf.T, 1, 0); // 1-->T
    for (int i = 1; i <= n; i++) {
        scanf("%ld", &a[i]);
        mcmf.add(mcmf.S, i+n, 1, a[i]);
        mcmf.add(i+n, i, 1, 0);
        mcmf.add(i, last[i], 1, 0);
    }
    for (int i = 1, x, y; i <= m; i++) {
        scanf("%d%d", &x, &y);
        int nx = ++mcmf.n, ny = ++mcmf.n;
        mcmf.add(x, nx, 1, 0), mcmf.add(y, ny, 1, 0);
        mcmf.add(last[x], ny, 1, 0), mcmf.add(last[y], nx, 1, 0);
        if (x == 1) mcmf.add(nx, mcmf.T, 1, 0);
        if (y == 1) mcmf.add(ny, mcmf.T, 1, 0);
        last[x] = nx, last[y] = ny;
    }
    printf("%ld\n", MCMF(mcmf)().second);
    for (int i = 1, u, v; i <= q; i++) {
        scanf("%d%d", &u, &v);
        a[u] += v;
        mcmf.add(mcmf.S, u+n, 1, a[u]);
        /* for (int j = 0; j <= tmp.n; j++) {
            for (const auto &[vv, bak, cap, cost, flg] : tmp.g[j])
                if (flg ) printf("%d---[%ld]->%d (%d)\n", j, cost, vv, !cap);
        } */
        printf("%ld\n", MCMF(mcmf)().second);
    }
    return 0;
}