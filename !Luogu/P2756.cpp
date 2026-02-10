#include <bits/stdc++.h>
constexpr int N = 105;
template<typename Tp>
struct MaxFlow {
    MaxFlow(int n_, int s, int t) : g(new EdgeList[n_+1]), dis(new int[n_+1]), head(new int[n_+1]), n(n_), S(s), T(t), maxflow() {}
    ~MaxFlow() { delete[] g; delete[] dis; delete[] head; }
    inline void addedg(int fr, int to, Tp c) {
        g[fr].emplace_back(to, c, (int)g[to].size());
        g[to].emplace_back(fr, Tp(0), (int)g[fr].size() - 1);
    }
    inline bool bfs() {
        static int que[N * N];
        memset(dis, 0, sizeof(int) * (n+1));
        memset(head, 0, sizeof(int) * (n+1));
        int hd = 0, tl = 0;
        que[tl++] = S; dis[S] = 1;
        while (hd ^ tl) {
            int u = que[hd++];
            for (const auto &[v, cap, bak] : g[u])
                if (cap && !dis[v]) dis[v] = dis[u] + 1, que[tl++] = v;
        }
        return dis[T];
    }
    Tp dfs(int u, Tp infl) {
        if (u == T) return infl;
        Tp outfl{};
        for (int &i = head[u]; i ^ (int)(g[u].size()); i++) {
            auto &[v, cap, bak] = g[u][i];
            if (cap && dis[v] == dis[u] + 1) {
                Tp fl = dfs(v, std::min(cap, infl));
                if (fl) {
                    infl -= fl, outfl += fl;
                    cap -= fl, std::get<1>(g[v][bak]) += fl;
                } else dis[v] = 0;
            }
        }
        return outfl;
    }
    inline Tp operator()() {
        if (maxflow) return maxflow;
        while (bfs()) maxflow += dfs(S, std::numeric_limits<Tp>::max());
        return maxflow;
    }
    using EdgeList = std::vector<std::tuple<int, Tp, int>>;
    EdgeList *g;
    int *dis, *head, n, S, T;
    Tp maxflow;
};
namespace Binary {
inline std::vector<std::pair<int,int>> match(MaxFlow<int> &mf, int n, int m) {
    int S = n+1, T = n+2;
    for (int i = 1; i <= n; i++)
        if (i <= m) mf.addedg(S, i, 1);
        else mf.addedg(i, T, 1);
    std::vector<std::pair<int,int>> ans;
    ans.reserve(n - mf());
    for (int u = 1; u <= m; u++)
        for (const auto &[v, cap, bak] : mf.g[u])
            if (cap == 0 && m < v && v <= n) ans.emplace_back(u, v);
    return ans;
}
}
int main() {
    int m, n;
    scanf("%d%d", &m, &n);
    MaxFlow<int> mf(n+2, n+1, n+2);
    for (int u, v; scanf("%d%d", &u, &v) && ~u; )
        mf.addedg(u, v, 1);
    auto ans = std::move(Binary::match(mf, n, m));
    printf("%zu\n", ans.size());
    for (const auto &[u, v] : ans)
        printf("%d %d\n", u, v);
    return 0;
}