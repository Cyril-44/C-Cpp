#include <bits/stdc++.h>
constexpr int N = 505;
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
int a[N], f[N];
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    // 1. 朴素 DP
    int ans1 = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = 0; j < i; j++)
            if (a[j] <= a[i]) f[i] = std::max(f[i], f[j] + 1);
        ans1 = std::max(ans1, f[i]);
    }
    if (ans1 == 1) return printf("%d\n%d\n%d\n", 1, n, n), 0; // 不特判会有 S__oo__i__oo__T
    // 2. 只能用一次，考虑建图，连接 j-->i (a[j]<=a[i]且f[i]=f[j]+1)，容量为1，表示可以转移，求最大流，注意限制单个点的流量
    int ans2; {
    int S=0, T=1;
    MaxFlow<int> mf(2*n+1, S, T);
    for (int i = 1; i < n; i++)
        for (int j = i+1; j <= n; j++)
            if (a[i] <= a[j] && f[j] == f[i] + 1) {
                // printf("%d %d\n", i, j);
                mf.addedg(i << 1 | 1, j << 1, 1);
            }
    for (int i = 1; i <= n; i++) {
        mf.addedg(i << 1, i << 1 | 1, 1);
        if (f[i] == 1) mf.addedg(S, i << 1, 1);
        if (f[i] == ans1) mf.addedg(i << 1 | 1, T, 1);
    }
    ans2 = mf();}
    // 3. 1 和 n 用无数次，只需要去除 1 和 n 的流量限制就可以了
    int ans3; {
    int S=0, T=1;
    MaxFlow<int> mf(2*n+1, S, T);
    for (int i = 1; i < n; i++)
        for (int j = i+1; j <= n; j++)
            if (a[i] <= a[j] && f[j] == f[i] + 1)
                mf.addedg(i << 1 | 1, j << 1, 1);
    for (int i = 2; i < n; i++) {
        mf.addedg(i << 1, i << 1 | 1, 1);
        if (f[i] == 1) mf.addedg(S, i << 1, 1);
        if (f[i] == ans1) mf.addedg(i << 1 | 1, T, 1);
    }
    mf.addedg(1 << 1, 1 << 1 | 1, std::numeric_limits<int>::max());
    mf.addedg(n << 1, n << 1 | 1, std::numeric_limits<int>::max());
    mf.addedg(S, 1 << 1, std::numeric_limits<int>::max());
    if (f[n] == 1) mf.addedg(S, n << 1, std::numeric_limits<int>::max());
    if (f[1] == ans1) mf.addedg(1 << 1 | 1, T, std::numeric_limits<int>::max());
    if (f[n] == ans1) mf.addedg(n << 1 | 1, T, std::numeric_limits<int>::max());
    ans3 = mf();}
    printf("%d\n%d\n%d\n", ans1, ans2, ans3);
    return 0;
}