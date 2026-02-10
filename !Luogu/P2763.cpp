#include <bits/stdc++.h>
constexpr int N = 1005, K = 25;
template<typename Tp>
struct MaxFlow {
    MaxFlow(int n_, int s, int t) : g(new EdgeList[n_+1]), dis(new int[n_+1]), head(new int[n_+1]), n(n_), S(s), T(t), maxflow() {}
    ~MaxFlow() { delete[] g; delete[] dis; delete[] head; }
    inline void addedg(int fr, int to, Tp c) {
        g[fr].emplace_back(to, c, (int)g[to].size());
        g[to].emplace_back(fr, Tp(0), (int)g[fr].size() - 1);
    }
    inline bool bfs() {
        static int que[N + K];
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
std::vector<int> ans[K];
int main() {
    int k, n, m = 0;
    scanf("%d%d", &k, &n);
    int S = n+k+1, T = n+k+2;
    MaxFlow<int> mf(T, S, T);
    for (int i = 1; i <= n; i++)
        mf.addedg(S, i, 1);
    for (int i = 1; i <= k; i++) {
        int cataCnt;
        scanf("%d", &cataCnt);
        ans[i].reserve(cataCnt);
        mf.addedg(i + n, T, cataCnt);
        m += cataCnt;
    }
    for (int i = 1; i <= n; i++) {
        int p;
        scanf("%d", &p);
        for (int bel; p--; ) {
            scanf("%d", &bel);
            mf.addedg(i, bel + n, 1);
        }
    }
    if (mf() != m) puts("No Solution!");
    else {
        for (int i = 1; i <= n; i++)
            for (const auto &[v, cap, bak] : mf.g[i])
                if (n < v && v <= n+k && cap == 0)
                    ans[v - n].push_back(i);
        for (int i = 1; i <= k; i++) {
            printf("%d: ", i);
            for (int j : ans[i]) printf("%d ", j);
            putchar('\n');
        }
    }
    return 0;
}