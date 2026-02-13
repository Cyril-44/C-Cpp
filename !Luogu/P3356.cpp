#include <bits/stdc++.h>
constexpr int N = 40;
template<typename Tp>
struct MaxCostMaxFlow {
    MaxCostMaxFlow(int n_, int s, int t) : g(new EdgeList[n_+1]), head(new int[n_+1]), dis(new Tp[n_+1]), vis(new bool[n_+1]), n(n_), S(s), T(t), maxflow(), maxcost() {}
    ~MaxCostMaxFlow() { delete[] g; delete[] head; delete[] dis; }
    inline void operator()(int fr, int to, Tp c1, Tp c2) {
        g[fr].emplace_back(to, c1, c2, (int)g[to].size());
        g[to].emplace_back(fr, 0, -c2, (int)g[fr].size() - 1);
    }
    inline bool spfa() {
        static int que[N * N];
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
int map[N][N];
int f[N][N];
int g[N*N][2];
/*
必须拆点，
限制一个石头只能取一次
*/
int main() {
    int c, n, m;
    scanf("%d%d%d", &c, &m, &n);
    int S=0, T=1;
    auto getid = [m](int x, int y) { return (x-1) * m + y; };
    MaxCostMaxFlow<int> mcmf(2*n*m+1, S, T);
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++) {
            scanf("%d", &map[i][j]);
            switch (map[i][j]) {
            case 2:
                mcmf(getid(i, j) << 1, getid(i, j) << 1 | 1, 1, 1);
            case 0:
                if (i > 1) mcmf(getid(i-1, j) << 1 | 1, getid(i, j) << 1, c, 0);
                if (j > 1) mcmf(getid(i, j-1) << 1 | 1, getid(i, j) << 1, c, 0);
                mcmf(getid(i, j) << 1, getid(i, j) << 1 | 1, c, 0);
                break;
            }
        }
    mcmf(S, getid(1, 1) << 1, c, 0);
    mcmf(getid(n, m) << 1 | 1, T, c, 0);

    auto [mf, mc] = mcmf();
    fprintf(stderr, "Max bots: %d\nMax stones collected: %d\n", mf, mc);
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            for (const auto &[v, cap, cost, bak] : mcmf.g[getid(i, j) << 1])
                if (cap > 0) {
                    if (i > 1 && v == (getid(i-1, j) << 1 | 1)) {
                        if (cost) fprintf(stderr, "(%d,%d)->(%d,%d) made cost + 1\n", i-1, j, i, j);
                        g[v >> 1][0] += cap;
                    }
                    if (j > 1 && v == (getid(i, j-1) << 1 | 1)) {
                        if (cost) fprintf(stderr, "(%d,%d)->(%d,%d) made cost + 1\n", i, j-1, i, j);
                        g[v >> 1][1] += cap;
                    }
                }
    auto next = [m](int x) {
        if (g[x][0]) return --g[x][0], x + m;
        if (g[x][1]) return --g[x][1], x + 1;
        return 0;
    };
    fprintf(stderr, "\t"); for (int i = 1; i <= m; i++) fprintf(stderr, "  %d   ", i); fprintf(stderr, "\n");
    for (int i = 1; i <= n; i++) {
        fprintf(stderr, "%d\t", i);
        for (int j = 1; j <= m; j++)
            fprintf(stderr, "%c%d,%d%c%c", "([{"[(map[i][j]>=1)+(map[i][j]>=2)], g[getid(i,j)][0], g[getid(i,j)][1], ")]}"[(map[i][j]>=1)+(map[i][j]>=2)], " \n"[j==m]);
    }
    for (int times = 1; times <= mf; times++) {
        std::vector<int> path;
        for (int u = getid(1, 1); u; u = next(u)) path.push_back(u);
        if (path.back() == getid(n, m)) {
            for (int i = 1; i < (int)path.size(); i++)
                if (path[i] - path[i-1] == 1) printf("%d 1\n", times);
                else printf("%d 0\n", times);
        } else --times;
        for (int i = 1; i < (int)path.size(); i++)
            if (path[i] - path[i-1] == 1) fprintf(stderr, "R");
            else fprintf(stderr, "D");
        fprintf(stderr, " At time %d, last = (%d,%d)\n", times, (path.back()-1)/m, (path.back()-1)%m+1);
    }
    return 0;
}