#include <stdio.h>
#include <algorithm>
#include <string.h>
#include <limits>
#include <vector>
#include <array>
#include <bitset>
#include <tuple>
#include <set>
constexpr int N = 105;
using Graph = std::array<std::bitset<N>, N>;
struct MaxFlow {
    MaxFlow(int n_, int s, int t) : g(new EdgeList[n_+1]), dis(new int[n_+1]), head(new int[n_+1]), n(n_), S(s), T(t), maxflow() {}
    ~MaxFlow() { delete[] g; delete[] dis; delete[] head; }
    inline void addedg(int fr, int to, int c) {
        g[fr].emplace_back(to, c, (int)g[to].size());
        g[to].emplace_back(fr, 0, (int)g[fr].size() - 1);
    }
    inline bool bfs() {
        memset(dis, 0, sizeof(int) * (n+1));
        memset(head, 0, sizeof(int) * (n+1));
        static int que[N << 1];
        int hd = 0, tl = 0;
        que[tl++] = S, dis[S] = 1;
        while (hd < tl) {
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
    int dfs(int u, int infl) {
        if (u == T) return infl;
        int outfl = 0;
        for (int &i = head[u]; i < (int)g[u].size(); i++) {
            auto &[v, cap, bak] = g[u][i];
            if (cap && dis[v] == dis[u] + 1) {
                int fl = dfs(v, std::min(infl, cap));
                if (fl) {
                    cap -= fl, std::get<1>(g[v][bak]) += fl;
                    infl -= fl, outfl += fl;
                } else dis[v] = -1;
            }
        }
        return outfl;
    }
    inline int operator()() {
        if (maxflow) return maxflow;
        while (bfs()) maxflow += dfs(S, std::numeric_limits<decltype(maxflow)>::max());
        return maxflow;
    }
    using EdgeList = std::vector<std::tuple<int,int,int>>;
    EdgeList *g;
    int *dis, *head, n, S, T, maxflow;
};
std::bitset<N> del;
namespace Solver {
std::vector<int> edgs[N << 1];
bool vis[N << 1];
void dfs(int u) {
    if (vis[u]) return;
    vis[u] = true;
    for (int v : edgs[u]) dfs(v);
}
inline int solve(const Graph &g, int n, bool needprint = false) {
    int S = 2*n + 1, T = 2*n + 2;
    MaxFlow mf(T, S, T);
    auto sender = [](int x) { return (x << 1) - 1; };
    auto receiver = [](int x) { return x << 1; };
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            if (g[i][j]) mf.addedg(sender(i), receiver(j), 1);
    for (int i = 1; i <= n; i++) if (!del[i])
        mf.addedg(S, sender(i), 1), mf.addedg(receiver(i), T, 1);
    if (needprint) {
        printf("%d\n", n - mf());
        for (int i = 1; i ^ S; i++)
            edgs[i].clear();
        memset(vis, 0, sizeof(bool) * S);
        std::set<int> st;
        for (int i = 1; i <= n; i++) st.insert(receiver(i));
        for (int i = 1; i <= n; i++) {
            int u = sender(i);
            for (const auto &[v, cap, bak] : mf.g[u])
                if (cap) edgs[v].push_back(u);
                else edgs[u].push_back(v), st.erase(v);
        }
        for (int i : st) dfs(i);
        for (int i = 1; i <= n; i++)
            putchar((!vis[sender(i)] && vis[receiver(i)]) | '0');
        putchar('\n');
    }
    return n - mf();
}
}
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    Graph adj;
    for (int u, v; m--; ) {
        scanf("%d%d", &u, &v);
        adj[u].set(v);
    }
    for (int k = 1; k <= n; k++)
        for (int i = 1; i <= n; i++)
            if (adj[i][k]) adj[i] |= adj[k];
    int ans;
    ans = Solver::solve(adj, n, true);
    for (int i = 1; i <= n; i++) { // 删掉 i 所在的所有链（即与i相连的所有点）之后跑最长反链，如果答案=ans-1那么就可能存在于最长反链中
        Graph tmp(adj);
        del.reset();
        for (int j = 1; j <= n; j++)
            if (i != j && (tmp[i][j] || tmp[j][i]))
                del.set(j), tmp[j].reset();
        del.set(i), tmp[i].reset();
        putchar((Solver::solve(tmp, n) - del.count() == ans - 1) | '0');
        // printf(" %d-%d\n", Solver::solve(tmp, n), (int)del.count());
    }
    putchar('\n');
    return 0;
}