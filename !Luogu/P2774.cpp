#include <bits/stdc++.h>
constexpr int N = 105;
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
/* 经典建模！！！！
考虑进行黑白染色，黑点连白点+oo，S连黑点 点权，白点连T 点权。
考虑使用“割”建模。
如果一个黑点属于 S，那么这个黑点与其相连的白点就不能连接到 T，否则 S 含 T，这就是互斥关系。
最后要求的是最大的没有被割掉的，两边的（S那一边和T那一边）的，边权的和，也就是总和-最小割
*/
int a[N][N];
int main() {
    int n, m, sum = 0;
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            scanf("%d", &a[i][j]), sum += a[i][j];
    auto getid = [m](int x, int y) { return (x - 1) * m + y; };
    int S = 0, T = n*m + 1;
    MaxFlow<int> mf(T, S, T);
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            if ((i&1)^(j&1)) mf.addedg(getid(i, j), T, a[i][j]);
            else {
                for (const auto &[dx, dy] : {std::pair<int,int>{-1,0},{0,1},{1,0},{0,-1}}) {
                    int tx = i + dx, ty = j + dy;
                    if (1 <= tx && tx <= n && 1 <= ty && ty <= m)
                        mf.addedg(getid(i, j), getid(tx, ty), std::numeric_limits<int>::max());
                }
                mf.addedg(S, getid(i, j), a[i][j]);
            }
    printf("%d\n", sum - mf());
    return 0;
}