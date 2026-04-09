#include <bits/stdc++.h>
struct MaxCostMaxFlow {
    MaxCostMaxFlow(int n, int s, int t) : g(n+1), head(n+1), dis(n+1), vis(n+1), maxcost(), maxflow(), n(n), S(s), T(t) {}
    inline bool bfs() {
        memset(dis.data(), 0x80, sizeof(int) * (n+1));
        memset(head.data(), 0, sizeof(int) * (n+1));
        dis[S] = 0;
        for (std::queue<int> q({S}); !q.empty(); q.pop()) {
            int u = q.front(); vis[u] = false;
            for (const auto &[v, bak, cap, cost] : g[u])
                if (cap && dis[v] < dis[u] + cost) {
                    dis[v] = dis[u] + cost;
                    if (!vis[v]) q.push(v), vis[v] = true;
                }
        }
        return dis[T] != (int)0x80808080;
    }
    inline void add(int fr, int to, int cap, int cost) {
        g[fr].emplace_back(to, g[to].size(), cap, cost);
        g[to].emplace_back(fr, g[fr].size() - 1, 0, -cost);
    }
    int dfs(int u, int infl) {
        if (u == T) return infl;
        vis[u] = true;
        int outfl = 0;
        for (int &i = head[u]; i < (int)g[u].size(); i++) {
            auto &[v, bak, cap, cost] = g[u][i];
            if (cap && !vis[v] && dis[v] == dis[u] + cost) {
                int fl = dfs(v, std::min(infl, cap));
                if (fl) {
                    maxcost += cost * fl;
                    outfl += fl, infl -= fl;
                    cap -= fl, std::get<2>(g[v][bak]) += fl;
                    if (!infl) break;
                }
            }
        }
        vis[u] = false;
        return outfl;
    }
    inline std::pair<int,int> operator()() {
        if (__builtin_expect(maxcost, 0)) return {maxflow, maxcost};
        while (bfs()) maxflow += dfs(S, std::numeric_limits<int>::max());
        return {maxflow, maxcost};
    }
    std::vector<std::vector<std::tuple<int,int,int,int>>> g;
    std::vector<int> head, dis;
    std::vector<bool> vis;
    int maxcost, maxflow;
    const int n, S, T;
};

int main() {
    int n, k;
    scanf("%d%d", &n, &k);
    std::vector<std::tuple<int,int,int>> ranges(n);
    std::vector<int> all(n * 2);
    for (int i = 0; i < n; i++) {
        int x0, y0, x1, y1;
        scanf("%d%d%d%d", &x0, &y0, &x1, &y1);
        if (x0 > x1) std::swap(x0, x1), std::swap(y0, y1);
        all[i<<1] = x0;
        all[i<<1|1] = x1;
        ranges[i] = {x0, x1, std::floor(std::sqrt(1ll*(x1-x0)*(x1-x0) + 1ll*(y1-y0)*(y1-y0)))};
    }
    std::sort(all.begin(), all.end());
    all.erase(std::unique(all.begin(), all.end()), all.end());
    int S = all.size() << 1, T = all.size() << 1 | 1;
    MaxCostMaxFlow mcmf(T, S, T);
    mcmf.add(S, 0, k, 0);
    for (int i = 1; i < (int)all.size() * 2; i++)
        mcmf.add(i-1, i, k, 0);
    for (int i = 0; i < n; i++) {
        int l = int(std::lower_bound(all.begin(), all.end(), std::get<0>(ranges[i])) - all.begin()) << 1,
            r = int(std::lower_bound(all.begin(), all.end(), std::get<1>(ranges[i])) - all.begin()) << 1;
        (l == r) ? (++r) : (++l);
        mcmf.add(l, r, 1, std::get<2>(ranges[i]));
    }
    mcmf.add(S-1, T, k, 0);
    printf("%d\n", mcmf().second);
    return 0;
}
