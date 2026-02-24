#include <bits/stdc++.h>
#include <limits>
constexpr int N = 105;
template<typename Tp>
struct MinCostMaxFlow {
    MinCostMaxFlow(int n_, int s, int t) : g(new EdgeList[n_+1]), head(new int[n_+1]), dis(new Tp[n_+1]), vis(new bool[n_+1]), n(n_), S(s), T(t), maxflow(), mincost() {}
    ~MinCostMaxFlow() { delete[] g; delete[] head; delete[] dis; }
    inline void operator()(int fr, int to, Tp c1, Tp c2) {
        g[fr].emplace_back(to, c1, c2, (int)g[to].size());
        g[to].emplace_back(fr, 0, -c2, (int)g[fr].size() - 1);
    }
    inline bool spfa() {
        static int que[N * N << 1];
        memset(head, 0, sizeof(int) * (n+1));
        memset(dis, 0x3f, sizeof(Tp) * (n+1));
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
        return dis[T] != (Tp)0x3f3f3f3f3f3f3f3fll;
    }
    Tp dfs(int u, Tp infl) {
        if (u == T) return infl;
        vis[u] = true;
        Tp outfl = 0;
        for (int &i = head[u]; i < (int)g[u].size(); i++) {
            auto &[v, cap, cost, bak] = g[u][i];
            if (cap && !vis[v] && dis[v] == dis[u] + cost) {
                Tp fl = dfs(v, std::min(infl, cap));
                if (fl) {
                    mincost += fl * cost;
                    infl -= fl, outfl += fl;
                    cap -= fl, std::get<1>(g[v][bak]) += fl;
                    if (!infl) return outfl;
                }
            }
        }
        vis[u] = false;
        return outfl;
    }
    inline std::pair<Tp,Tp> operator()() {
        if (maxflow) return {maxflow, mincost};
        while (spfa()) maxflow += dfs(S, std::numeric_limits<Tp>::max());
        return {maxflow, mincost};
    }
    using EdgeList = std::vector<std::tuple<int,Tp,Tp,int>>;
    EdgeList *g;
    int *head;
    Tp *dis;
    bool *vis;
    int n, S, T;
    Tp maxflow, mincost;
};
/*
常见套路：如果仓库货物比最终的要多，就连 T 多的量，反之 S 连 少的量。
然后两两相邻的转移即可
*/
int a[N];
int main() {
    int n, sum = 0;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]), sum += a[i];
    int avg = sum / n;
    int S = 0, T = n+1;
    MinCostMaxFlow<int> mcmf(T, S, T);
    for (int i = 1; i <= n; i++)
        if (a[i] > avg) mcmf(i, T, a[i] - avg, 0);
        else mcmf(S, i, avg - a[i], 0);
    for (int i = 1; i <= n; i++) {
        mcmf(i, i%n+1, std::numeric_limits<int>::max(), 1);
        mcmf(i%n+1, i, std::numeric_limits<int>::max(), 1);
    }
    printf("%d\n", mcmf().second);
}