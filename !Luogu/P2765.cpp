#include <bits/stdc++.h>
#include <cassert>
constexpr int N = 10000;
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
        static int que[N + 5 << 1];
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
/* 最小路径覆盖建模！
题意可以先转化为求最小的 x 使得将 1~x 可以按照要求分成 n 组之。
想到可以使用二分
然后变成 DAG 建模，每个数可以与它的对偶连边，然后。。。然后就可以做了。
*/
int to[N];
bool hasin[N];
int main() {
    int n;
    scanf("%d", &n);
    int S = 0, T = 1;
    MaxFlow<int> mf(4001, S, T);
    for (int i = 1; i <= 2000; i++)
        for (int j = 1; ; j++)
            if (j * j - i > i) {
                if (j * j - i > 2000) break;
                mf.addedg(i << 1, j * j - i << 1 | 1, 1);
                // fprintf(stderr, "Added %d-->%d\n", i, j*j-i);
            }
    int x = 0;
    while (x - mf() <= n) {
        ++x;
        mf.addedg(S, x << 1, 1);
        mf.addedg(x << 1 | 1, T, 1);
    }
    // 最后一个 x 是不行的 不能要
    for (int u = 1; u < x; u++)
        for (const auto &[v, cap, bak] : mf.g[u << 1])
            if (cap == 0 && v != S && v != T) {
                to[u] = v >> 1, hasin[v >> 1] = true;
                break;
            }
    printf("%d\n", x - 1);
    for (int i = 1; i < x; i++) {
        if (!hasin[i]) {
            for (int u = i; u; u = to[u])
                printf("%d ", u);
            putchar('\n');
        }
    }
    return 0;
}