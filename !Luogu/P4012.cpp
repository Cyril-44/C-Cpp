#include <bits/stdc++.h>
constexpr int N = 15;
template<typename Tp>
struct MinCostMaxFlow {
    MinCostMaxFlow(int n_, int s, int t) : g(new EdgeList[n_+1]), head(new int[n_+1]), dis(new Tp[n_+1]), vis(new bool[n_+1]), n(n_), S(s), T(t), maxflow(), maxcost() {}
    ~MinCostMaxFlow() { delete[] g; delete[] head; delete[] dis; }
    inline void operator()(int fr, int to, Tp c1, Tp c2) {
        g[fr].emplace_back(to, c1, c2, (int)g[to].size());
        g[to].emplace_back(fr, 0, -c2, (int)g[fr].size() - 1);
    }
    inline bool spfa() {
        static int que[N * N << 3];
        memset(head, 0, sizeof(int) * (n+1));
        memset(dis, 0x80, sizeof(Tp) * (n+1));
        memset(vis, 0, sizeof(bool) * (n+1));
        int hd = 0, tl = 0;
        que[tl++] = S, dis[S] = 0;
        while (hd ^ tl) {
            int u = que[hd++];
            vis[u] = false;
            for (const auto &[v, cap, cost, bak] : g[u]) {
                if (cap && dis[v] < dis[u] + cost) {
                    dis[v] = dis[u] + cost;
                    if (!vis[v]) que[tl++] = v;
                }
            }
        }
        return dis[T] != (Tp)0x8080808080808080ll;
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
                    maxcost += fl * cost;
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
        if (maxflow) return {maxflow, maxcost};
        while (spfa()) maxflow += dfs(S, std::numeric_limits<Tp>::max());
        return {maxflow, maxcost};
    }
    using EdgeList = std::vector<std::tuple<int,Tp,Tp,int>>;
    EdgeList *g;
    int *head;
    Tp *dis;
    bool *vis;
    int n, S, T;
    Tp maxflow, maxcost;
};
int main() {
    int a, b, p, q;
    scanf("%d%d%d%d", &a, &b, &p, &q);
    auto getid = [q](int x, int y) { return x * (q+1) + y; };
    int S = p * q * 2 + 1, T = p * q * 2 + 2;
    MinCostMaxFlow<int> mcmf(T, S, T);
    for (int i = 0; i <= p; i++)
        for (int j = 0; j < q; j++) {
            int val; scanf("%d", &val);
            mcmf(getid(i, j), getid(i, j+1), 1, val);
            mcmf(getid(i, j), getid(i, j+1), std::numeric_limits<int>::max(), 0);
        }
    for (int j = 0; j <= q; j++)
        for (int i = 0; i < p; i++) {
            int val; scanf("%d", &val);
            mcmf(getid(i, j), getid(i+1, j), 1, val);
            mcmf(getid(i, j), getid(i+1, j), std::numeric_limits<int>::max(), 0);
        }
    for (int i = 1; i <= a; i++) {
        int r, x, y; scanf("%d%d%d", &r, &x, &y);
        mcmf(S, getid(x, y), r, 0);
    }
    for (int i = 1; i <= b; i++) {
        int r, x, y; scanf("%d%d%d", &r, &x, &y);
        mcmf(getid(x, y), T, r, 0);
    }
    auto [mf, mc] = mcmf();
    printf("%d\n", mc);
}
