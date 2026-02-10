#include <bits/stdc++.h>
constexpr int N = 55;
/*
最小割建模，最小化 {失去实验的费用 + 添加器材的费用}
*/
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
        while (bfs()) maxflow += dfs(S, std::numeric_limits<Tp>::max());
        return maxflow;
    }
    using EdgeList = std::vector<std::tuple<int, Tp, int>>;
    EdgeList *g;
    int *dis, *head, n, S, T;
    Tp maxflow;
};
char buffer[1 << 10];
int currentIdx;
inline void readline() {
    fgets(buffer, sizeof buffer, stdin);
    currentIdx = 0;
}
inline void in(int &x) {
    int adds;
    if (sscanf(buffer + currentIdx, "%d%n", &x, &adds) != 1) x = -1;
    else currentIdx += adds;
}
int main() {
    int n, m; // n 个仪器
    scanf("%d%d ", &m, &n);
    int T = m+n+1, S = m+n+2;
    MaxFlow<long long> mf(S, S, T);
    long long sump = 0;
    for (int j = 1; j <= m; j++) {
        readline();
        int pj; in(pj); sump += pj;
        mf.addedg(S, j+n, pj); // 直接不要这个实验，减去 pj
        for (int ri; in(ri), ~ri; )
            mf.addedg(j + n, ri, std::numeric_limits<long long>::max());
    }
    for (int i = 1; i <= n; i++) {
        int ci; scanf("%d", &ci);
        mf.addedg(i, T, ci);
    }
    mf();
    /*
    1~n 表示仪器，连接到 T，如果边 cap=0 那就是割掉了，也就是要这个仪器
    n+1~n+m 表示实验，连接到 S，如果边 cap!=0 那就是保留，要这个实验
    现在从实验那一边开始跑 getdep, 如果 dep!=0 那就是需要这个仪器/实验
    */
    for (int i = n+1; i <= n+m; i++)
        if (mf.dis[i]) printf("%d ", i - n);
    putchar('\n');
    for (int i = 1; i <= n; i++)
        if (mf.dis[i]) printf("%d ", i);
    putchar('\n');
    printf("%lld\n", sump - mf());
    return 0;
}