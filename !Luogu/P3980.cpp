#include <bits/stdc++.h>
template<std::signed_integral TCap, std::signed_integral TCost = TCap, class Comp = std::less<TCost>>
requires std::predicate<Comp, TCost, TCost>
struct MCostFlow {
    constexpr static TCost InitVal = Comp{}(TCost(0), TCost(1)) ? std::numeric_limits<TCost>::max() : std::numeric_limits<TCost>::min();
    MCostFlow(int n, int s, int t) : g(n+1), dis(n+1), head(n+1), vis(n+1), maxflow(), mcost(), n(n), S(s), T(t) {}
    inline bool spfa() {
        dis.assign(n+1, InitVal);
        head.assign(n+1, 0);
        vis.assign(n+1, false);
        dis[S] = 0;
        for (std::queue<int> q({S}); !q.empty(); q.pop()) {
            int u = q.front();
            vis[u] = false;
            for (const auto &[v, bak, cap, cost] : g[u])
                if (cap && Comp{}(dis[u] + cost, dis[v])) {
                    dis[v] = dis[u] + cost;
                    if (!vis[v]) q.push(v), vis[v] = true;
                }
        }
        return dis[T] != InitVal;
    }
    inline void add(int fr, int to, TCap cap, TCost cost) {
        g[fr].emplace_back(to, g[to].size(), cap, cost);
        g[to].emplace_back(fr, g[fr].size() - 1, 0, -cost);
    }
    TCap dfs(int u, TCap infl) {
        vis[u] = true;
        if (u == T) return infl;
        TCap outfl = 0;
        for (int &i = head[u]; i < (int)g[u].size(); i++) {
            auto &[v, bak, cap, cost] = g[u][i];
            if (cap && !vis[v] && dis[v] == dis[u] + cost) {
                TCap fl = dfs(v, std::min(infl, cap));
                if (fl) {
                    mcost += fl * cost;
                    infl -= fl, outfl += fl;
                    cap -= fl, std::get<2>(g[v][bak]) += fl;
                    if (!infl) break;
                } else dis[v] = InitVal;
            }
        }
        return outfl;
    }
    inline std::pair<TCap, TCost> operator()() {
        while (spfa()) {
            do vis.assign(n+1, false), maxflow += dfs(S, std::numeric_limits<TCap>::max());
            while (vis[T]);
        }
        return {maxflow, mcost};
    }
    std::vector<std::vector<std::tuple<int,int,TCap, TCost>>> g;
    std::vector<TCost> dis;
    std::vector<int> head;
    std::vector<bool> vis;
    TCap maxflow;
    TCost mcost;
    int n, S, T;
};
int main() {
    constexpr int INF = 0x7fffffff;
    int n, m;
    scanf("%d%d", &n, &m);
    int S = 0, T = n + 2;
    MCostFlow<int,int64_t> mcmf(T, S, T);
    mcmf.add(S, 1, INF, 0), mcmf.add(n+1, T, INF, 0);
    for (int i = 1, ai; i <= n; i++) {
        scanf("%d", &ai);
        mcmf.add(i, i+1, INF - ai, 0);
    }
    for (int si, ti, ci; m--;) {
        scanf("%d%d%d", &si, &ti, &ci);
        mcmf.add(si, ti+1, INF, ci);
    }
    printf("%ld\n", mcmf().second);
}
