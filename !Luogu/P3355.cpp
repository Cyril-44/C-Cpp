#include <bits/stdc++.h>
constexpr int N = 205;
template<typename Tp>
struct MaxFlow {
    MaxFlow(int n_, int s, int t) : g(new EdgeList[n_+1]), dis(new int[n_+1]), head(new int[n_+1]), n(n_), S(s), T(t), modified(false), maxflow() {}
    ~MaxFlow() { delete[] g; delete[] dis; delete[] head; }
    inline void addedg(int fr, int to, Tp c) {
        modified = true;
        g[fr].emplace_back(to, c, (int)g[to].size());
        g[to].emplace_back(fr, Tp(0), (int)g[fr].size() - 1);
    }
    inline bool bfs() {
        modified = true;
        static int que[N * N];
        memset(dis, 0, sizeof(int) * (n+1));
        memset(head, 0, sizeof(int) * (n+1));
        int hd = 0, tl = 0;
        que[tl++] = S; dis[S] = 1;
        while (hd ^ tl) {
            int u = que[hd++];
            for (const auto &[v, cap, bak] : g[u])
                if (cap && !dis[v]) {
                    dis[v] = dis[u] + 1;
                    if (v == T) return true;
                    que[tl++] = v;
                }
        }
        return false;
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
                } else dis[v] = -1;
            }
        }
        return outfl;
    }
    inline Tp operator()() {
        if (!modified) return maxflow;
        while (bfs()) maxflow += dfs(S, std::numeric_limits<Tp>::max());
        return maxflow;
    }
    using EdgeList = std::vector<std::tuple<int, Tp, int>>;
    EdgeList *g;
    int *dis, *head, n, S, T;
    bool modified;
    Tp maxflow;
};
bool block[N][N];
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    int S=0, T=n*n+1;
    MaxFlow<int> mf(T, S, T);
    for (int x, y, i=1; i<=m; i++) {
        scanf("%d%d", &x, &y);
        block[x][y] = true;
    }
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            if ((i&1)^(j&1)){
                if (!block[i][j]) {
                    mf.addedg(S, (i-1)*n+j, 1);
                    for (const auto &[dx, dy] : {std::pair<int,int>{-2,1},{-1,2},{1,2},{2,1},{1,-2},{2,-1},{-1,-2},{-2,-1}}) {
                        int tx = i + dx, ty = j + dy;
                        if (1 <= tx && tx <= n && 1 <= ty && ty <= n && !block[tx][ty])
                            mf.addedg((i-1)*n+j, (tx-1)*n+ty, n*n-m);
                    }
                }
            } else {
                if (!block[i][j]) mf.addedg((i-1)*n+j, T, 1);
            }
    printf("%d\n", n*n - m - mf());
    return 0;
}