#include <bits/stdc++.h>
#include <limits>
constexpr int N = 55;
template<typename Tp>
struct MaxCostMaxFlow {
    MaxCostMaxFlow(int n_, int s, int t) : g(new EdgeList[n_+1]), head(new int[n_+1]), dis(new Tp[n_+1]), vis(new bool[n_+1]), n(n_), S(s), T(t), maxflow(), maxcost() {}
    ~MaxCostMaxFlow() { delete[] g; delete[] head; delete[] dis; }
    inline void operator()(int fr, int to, Tp c1, Tp c2) {
        g[fr].emplace_back(to, c1, c2, (int)g[to].size());
        g[to].emplace_back(fr, 0, -c2, (int)g[fr].size() - 1);
    }
    inline bool spfa() {
        static int que[N * N << 2];
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
int a[N][N+N];
int main() {
    int m, n;
    scanf("%d%d", &m, &n);
    for (int i = 0; i < n; i++)
        for (int j = 1; j <= i+m; j++)
            scanf("%d", &a[i][j]);
    auto getid = [m](int x, int y) { return (m + m + x - 1) * x / 2 + y; };
    {   int S = 0, T = 1, All = 2*getid(n-1, n-1+m) + 1;
        MaxCostMaxFlow<int> mf(All, S, T);
        for (int i = 0; i < n; i++)
            for (int j = 1; j <= i+m; j++)
                mf(getid(i, j) << 1, getid(i, j) << 1 | 1, 1, a[i][j]);
        for (int j = 1; j <= m; j++)
            mf(S, getid(0, j) << 1, 1, 0);
        for (int j = 1; j < m+n; j++)
            mf(getid(n-1, j) << 1 | 1, T, 1, 0);
        for (int i = 1; i < n; i++)
            for (int j = 1; j < i+m; j++) {
                mf(getid(i-1, j) << 1 | 1, getid(i, j) << 1, 1, 0);
                mf(getid(i-1, j) << 1 | 1, getid(i, j + 1) << 1, 1, 0);
            }
        printf("%d\n", mf().second);
    }
    {   int S = 0, T = 1, All = 2*getid(n-1, n-1+m) + 1;
        MaxCostMaxFlow<int> mf(All, S, T);
        for (int i = 0; i < n; i++)
            for (int j = 1; j <= i+m; j++)
                mf(getid(i, j) << 1, getid(i, j) << 1 | 1, 2, a[i][j]);
        for (int j = 1; j <= m; j++)
            mf(S, getid(0, j) << 1, 1, 0);
        for (int j = 1; j < m+n; j++)
            mf(getid(n-1, j) << 1 | 1, T, 2, 0);
        for (int i = 1; i < n; i++)
            for (int j = 1; j < i+m; j++) {
                mf(getid(i-1, j) << 1 | 1, getid(i, j) << 1, 1, 0);
                mf(getid(i-1, j) << 1 | 1, getid(i, j + 1) << 1, 1, 0);
            }
        printf("%d\n", mf().second);
    }
    {   int S = 0, T = getid(n-1, n-1+m) + 1;
        MaxCostMaxFlow<int> mf(T, S, T);
        for (int i = 1; i <= m; i++) mf(S, getid(0, i), 1, a[0][i]);
        for (int i = 1; i < n; i++)
            for (int j = 1; j < i+m; j++) {
                mf(getid(i-1, j), getid(i, j), std::numeric_limits<int>::max(), a[i][j]);
                mf(getid(i-1, j), getid(i, j+1), std::numeric_limits<int>::max(), a[i][j+1]);
            }
        for (int j = 1; j < m+n; j++)
            mf(getid(n-1, j), T, std::numeric_limits<int>::max(), 0);
        printf("%d\n", mf().second);
    }
    return 0;
}