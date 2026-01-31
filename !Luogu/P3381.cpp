#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <tuple>
constexpr int N = 155, M = 6005;
struct MaxCostMaxFlow {
    MaxCostMaxFlow(int n) : g(new EdgeList[n+1]), dis(new int[n+1]), head(new int[n+1]), vis(new bool[n+1]()), memsetSz(sizeof(int) * (n+1)), S(), T(), maxflow(), maxcost() {}
    inline void init(int s, int t) { S = s, T = t; }
    std::pair<int,int> operator()() {
        if (!S || !T) return {-1,-1};
        if (maxflow) return {maxflow, maxcost};
        while (spfa()) maxflow += dfs(S, 0x7fffffff);
        return {maxflow, maxcost};
    }
    inline void addedg(int fr, int to, int cap, int cost) {
        g[fr].emplace_back(to, cap, cost, (int)g[to].size());
        g[to].emplace_back(fr, 0, -cost, (int)g[fr].size() - 1);
    }
private:
    inline bool spfa() {
        memset(dis, 0x3f, memsetSz);
        memset(head, 0, memsetSz);
        static int que[N * M];
        int l = 0, r = 0;
        que[r++] = S, dis[S] = 0;
        while (l < r) {
            int u = que[l++];
            vis[u] = false;
            for (const auto &[v, cap, cost, bak] : g[u])
                if (cap && dis[v] > dis[u] + cost) {
                    dis[v] = dis[u] + cost;
                    if (!vis[v]) vis[v] = true, que[r++] = v;
                }
        }
        return dis[T] != 0x3f3f3f3f;
    }
    int dfs(int u, int flow) {
        if (u == T) return flow;
        vis[u] = true;
        int sum = 0;
        for (int &i = head[u]; i < (int)g[u].size(); i++) {
            auto &[v, cap, cost, bak] = g[u][i];
            if (cap && !vis[v] && dis[v] == dis[u] + cost) {
                int fl = dfs(v, std::min(flow, cap));
                if (fl) {
                    flow -= fl, sum += fl;
                    cap -= fl, std::get<1>(g[v][bak]) += fl;
                    maxcost += fl * cost;
                }
            }
        }
        vis[u] = false;
        return sum;
    }
    using EdgeList = std::vector<std::tuple<int,int,int,int>>;
    EdgeList *g;
    int *dis, *head;
    bool *vis;
    size_t memsetSz;
    int S, T;
    int maxflow, maxcost;
};
int main() {
    int n, m, s, t;
    scanf("%d%d%d%d", &n, &m, &s, &t);
    MaxCostMaxFlow mcmf(n);
    mcmf.init(s, t);
    for (int u, v, w, c; m--; ) {
        scanf("%d%d%d%d", &u, &v, &w, &c);
        mcmf.addedg(u, v, w, c);
    }
    auto [mf, mc] = mcmf();
    printf("%d %d\n", mf, mc);
    return 0;
}