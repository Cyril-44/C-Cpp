#include <bits/stdc++.h>
#include <csetjmp>
constexpr int N = 305, M = 1005;
std::vector<int> g[N];
/*
考虑到如果是最小生成树，那么对于每一条树边(x_i)，其所属的所有环（这种环是由很多条树边和一条非树边组成的）内的非树边(y_i) 权值 都比树边小。
根据这个性质，我们知道树边一定只会减边权，非树边只会加边权。
x_i - dec_i <= y_j + inc_j
因为我们要求的是 dec_i 和 inc_j，拉到一起，dec_i + inc_j >= x_i - y_j 
转化为线性规划问题。求 min C_T*x，在条件 A*x >= B 下。
这里的矩阵 A 是 (限制条件数量)*m 的，B 是上式的每一个 (x_i - y_j)，C 是加减权值付出的代价
套路性的对于这个线性规划对偶。
现在是求 max B_T*y，在条件 A*y <= C 下。
如果我们想要跑网络流，希望点尽可能少，考虑将 AT 的每一行（也就是 A 的每一列，即每一条边）抽象成点。
注意到每一列只有两个点，而且这两个点一个是树边，一个是非树边。于是就想到这两个点连边，拿边的费用来记录每一个 B*y。
发现这样做是可行的，因为事实上我们发现类似于这样建图：S --> 树边 --> 非树边 --> T，对于表示“树边”的点的出边，其流量和可以用点的入边来限制，而对于非树边恰恰相反。
于是跑最大费用**可行流**就可以了。
*/
std::vector<std::tuple<int,int,int,int>> treeEdgs, nonTreeEdgs;
std::bitset<N> vis;
void dfs(int u) { vis[u] = true; for (int v : g[u]) if (!vis[v]) dfs(v); }
inline auto getcon(int u, int v) { vis.reset(); vis.set(v); dfs(u); vis.reset(v); return vis; }
struct MaxCostMaxFlow {
    using EdgeList = std::vector<std::tuple<int,int,int,int>>;
    bool bfs() {
        memset(dis, 0x80, sizeof(int) * (n+1));
        memset(head, 0, sizeof(int) * (n+1));
        memset(vis, 0, sizeof(bool) * (n+1));
        int l = 0, r = 0;
        auto pop = [&]() { int res = que[l++]; vis[res] = false; if (l == LMT) l = 0; return res; };
        auto push = [&](int val) { que[r++] = val; vis[val] = true; if (r == LMT) r = 0; };
        dis[S] = 0; push(S);
        while (l ^ r) {
            int u = pop();
            for (const auto &[v, bak, cap, cost] : g[u])
                if (cap && dis[v] < dis[u] + cost) {
                    dis[v] = dis[u] + cost;
                    if (!vis[v]) push(v);
                }
        }
        return dis[T] != (int)0x80808080;
    }
    int dfs(int u, int infl) {
        if (u == T) return infl;
        int outfl = 0;
        vis[u] = true;
        for (int &i = head[u]; i < (int)g[u].size(); i++) {
            auto &[v, bak, cap, cost] = g[u][i];
            if (cap && !vis[v] && dis[v] == dis[u] + cost) {
                int fl = dfs(v, std::min(infl, cap));
                if (fl) {
                    maxcost += 1ull * cost * fl;
                    infl -= fl, outfl += fl;
                    cap -= fl, std::get<2>(g[v][bak]) += fl;
                    if (!infl) return outfl; 
                } else dis[v] = (int)0x80808080;
            }
        }
        return outfl;
    }
    MaxCostMaxFlow(int n_, int S_, int T_) : vis(new bool[n_+1]), dis(new int[n_+1]), que(new int[(n_<<3)+1]), head(new int[n_+1]), g(new EdgeList[n_+1]), maxcost(), n(n_), S(S_), T(T_), LMT(n << 3), maxflow() {}
    ~MaxCostMaxFlow() { delete[] vis; delete[] dis; delete[] que; delete[] head; delete[] g; }
    inline std::pair<int, unsigned long long> operator()() {
        if (maxflow) goto ret;
        while (bfs()) maxflow += dfs(S, std::numeric_limits<int>::max());
        ret: return {maxflow, maxcost};
    }
    inline void operator()(int fr, int to, int cap, int cost) {
        // printf("%d ===> %d with Cap(%d), Cost(%d)\n", fr, to, cap, cost);
        g[fr].emplace_back(to, (int)g[to].size(), cap, cost);
        g[to].emplace_back(fr, (int)g[fr].size() - 1, 0, -cost);
    }
    bool *vis;
    int *dis, *que, *head;
    EdgeList *g;
    unsigned long long maxcost;
    int n, S, T, LMT, maxflow;
};
std::bitset<N> vis;
void dfs(int u) { vis[u] = true; for (int v : g[u]) if (!vis[v]) dfs(v); }
inline auto getcon(int u, int v) { vis.reset(); vis.set(v); dfs(u); return vis; }
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    treeEdgs.reserve(m), nonTreeEdgs.reserve(m);
    for (int i = 1, u, v, w, f, a, b; i <= m; i++) {
        scanf("%d%d%d%d%d%d", &u, &v, &w, &f, &a, &b);
        if (f) g[u].push_back(v), g[v].push_back(u);
        if (f) treeEdgs.emplace_back(u, v, w, b);
        else nonTreeEdgs.emplace_back(u, v, w, a);
    }
    int S = m+1, T = m+2, treeEdgIdx = 0;
    MaxCostMaxFlow mcmf(T, 0, T);
    for (const auto &[u, v, w, c] : treeEdgs) {
        ++treeEdgIdx; int nonTreeEdgIdx = n - 1;
        auto conu = getcon(u, v), conv = getcon(v, u);
        for (const auto &[nu, nv, nw, nc] : nonTreeEdgs) {
            ++nonTreeEdgIdx;
            if (conu[nu] && conv[nv] || conu[nv] && conv[nu])
                mcmf(treeEdgIdx, nonTreeEdgIdx, std::numeric_limits<int>::max(), w - nw);
        }
    }
    int idx = 0, allc = 0;
    for (const auto &[u, v, w, c] : treeEdgs) mcmf(S, ++idx, c, 0), allc += c;
    for (const auto &[u, v, w, c] : nonTreeEdgs) mcmf(++idx, T, c, 0);
    mcmf(0, S, allc, 0), mcmf(S, T, allc, 0); // 可行流
    auto [mf, mc] = mcmf();
    printf("%llu\n", mc);
    return 0;
}