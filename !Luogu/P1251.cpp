#include <bits/stdc++.h>
#define INF std::numeric_limits<LL>::max()
using LL = long long;
constexpr int N = 2005;
struct MinCostMaxFlow {
    MinCostMaxFlow(int n_, int s, int t) : g(new EdgeList[n_+1]), head(new int[n_+1]), dis(new LL[n_+1]), vis(new bool[n_+1]), n(n_), S(s), T(t), maxflow(), mincost() {}
    ~MinCostMaxFlow() { delete[] g; delete[] head; delete[] dis; }
    inline void operator()(int fr, int to, LL c1, LL c2) {
        g[fr].emplace_back(to, c1, c2, (int)g[to].size());
        g[to].emplace_back(fr, 0, -c2, (int)g[fr].size() - 1);
    }
    inline bool spfa() {
        static int que[N * N];
        memset(head, 0, sizeof(int) * (n+1));
        memset(dis, 0x3f, sizeof(LL) * (n+1));
        memset(vis, 0, sizeof(bool) * (n+1));
        int hd = 0, tl = 0;
        que[tl++] = S, dis[S] = 0;
        while (hd ^ tl) {
            int u = que[hd++];
            vis[u] = false;
            for (const auto &[v, cap, cost, bak] : g[u]) {
                if (cap && dis[v] > dis[u] + cost) {
                    dis[v] = dis[u] + cost;
                    if (!vis[v]) que[tl++] = v;
                }
            }
        }
        return dis[T] != 0x3f3f3f3f3f3f3f3fll;
    }
    LL dfs(int u, LL infl) {
        if (u == T) return infl;
        vis[u] = true;
        LL outfl = 0;
        for (int &i = head[u]; i < (int)g[u].size(); i++) {
            auto &[v, cap, cost, bak] = g[u][i];
            if (cap && !vis[v] && dis[v] == dis[u] + cost) {
                LL fl = dfs(v, std::min(infl, cap));
                if (fl) {
                    mincost += fl * cost;
                    infl -= fl, outfl += fl;
                    cap -= fl, std::get<1>(g[v][bak]) += fl;
                }
            }
        }
        vis[u] = false;
        return outfl;
    }
    inline std::pair<LL,LL> operator()() {
        if (maxflow) return {maxflow, mincost};
        while (spfa()) maxflow += dfs(S, INF);
        return {maxflow, mincost};
    }
    using EdgeList = std::vector<std::tuple<int,LL,LL,int>>;
    EdgeList *g;
    int *head;
    LL *dis;
    bool *vis;
    int n, S, T;
    LL maxflow, mincost;
};
int r[N];
int main() {
    int n, p, fastcd, fastc, slowcd, slowc;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) scanf("%d", &r[i]);
    scanf("%d%d%d%d%d", &p, &fastcd, &fastc, &slowcd, &slowc);
    int S = n*2+1, T = n*2 + 2;
    MinCostMaxFlow mcmf(T, S, T);
    auto server = [](int x) { return (x<<1) - 1; };
    auto planner = [](int x) { return x<<1; };
    // 这张网络流，使用了入流出流 恒等于 \sum r[i]
    for (int i = 1; i <= n; i++) { // 在 server 中的流量，要不然给 planner 也就是计划去洗，要不然给汇点，也就是购买之后再也不用了
        mcmf(server(i), T, r[i], 0); // 新毛巾，每天的新毛巾数量必须等于 r[i] 条
        mcmf(S, planner(i), r[i], 0); // 旧毛巾，当天消耗的毛巾，必须保证是 r[i] 条，不能由 server 给出，只能由源点给出
        mcmf(S, server(i), INF, p); // 源点可以提供旧毛巾，代价是 p
        if (i ^ n) mcmf(planner(i), planner(i+1), INF, 0); // 旧毛巾的递推
        if (i + fastcd <= n) mcmf(planner(i), server(i + fastcd), INF, fastc); // 从旧毛巾变成新毛巾
        if (i + slowcd <= n) mcmf(planner(i), server(i + slowcd), INF, slowc);
    }
    printf("%lld\n", mcmf().second);
    return 0;
}