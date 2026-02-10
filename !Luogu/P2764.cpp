#include <bits/stdc++.h>
constexpr int N = 155;
template<typename Tp>
struct MaxFlow {
    MaxFlow(int n_, int s, int t) : g(new EdgeList[n_+1]), dis(new int[n_+1]), head(new int[n_+1]), n(n_), S(s), T(t), maxflow() {}
    ~MaxFlow() { delete[] g; delete[] dis; delete[] head; }
    inline void addedg(int fr, int to, Tp c) {
        g[fr].emplace_back(to, c, (int)g[to].size());
        g[to].emplace_back(fr, Tp(0), (int)g[fr].size() - 1);
    }
    inline bool bfs() {
        static int que[N << 1];
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
int to[N];
bool hasin[N];
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    int S = n*2+1, T = n*2+2;
    MaxFlow<int> mf(T, S, T);
    for (int i = 1; i <= n; i++)
        mf.addedg(S, i, 1),
        mf.addedg(i+n, T, 1);
    for (int i = 1, u, v; i <= m; i++) {
        scanf("%d%d", &u, &v);
        mf.addedg(u, v+n, 1);
    }
    int nums = n - mf();
    for (int u = 1; u <= n; u++)
        for (const auto &[v, cap, bak] : mf.g[u])
            if (cap == 0 && n < v && v <= 2*n) to[u] = v - n, hasin[v - n] = true;
    for (int i = 1; i <= n; i++) {
        if (!hasin[i]) {
            for (int u = i; u; u = to[u])
                printf("%d ", u);
            putchar('\n');
        }
    }
    printf("%d\n", nums);
    return 0;
}