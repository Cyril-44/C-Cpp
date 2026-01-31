#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <tuple>
constexpr int N = 1005, M = 100005;
struct MaxFlow {
    MaxFlow(int n) : g(new EdgeList[n+1]), dis(new int[n+1]), head(new int[n+1]), memsetSz(sizeof(int) * (n+1)), S(), T(), maxflow() {}
    inline void init(int s, int t) { S = s, T = t; }
    inline int operator()() {
        if (!S || !T) return -1;
        if (maxflow) return maxflow;
        while (bfs()) maxflow += dfs(S, 0x7fffffff);
        return maxflow;
    }
    inline void addedg(int fr, int to, int cap) {
        g[fr].emplace_back(to, cap, (int)g[to].size());
        g[to].emplace_back(fr, 0, (int)g[fr].size() - 1);
    }
private:
    inline bool bfs() {
        static int que[N];
        memset(dis, 0, memsetSz);
        memset(head, 0, memsetSz);
        int l = 0, r = 0;
        que[r++] = S; dis[S] = 1;
        while (l < r) {
            int u = que[l++];
            for (const auto &[v, cap, bak] : g[u])
                if (cap && !dis[v]) {
                    dis[v] = dis[u] + 1;
                    if (v == T) return true;
                    que[r++] = v;
                }
        }
        return false;
    }
    int dfs(int u, int flow) {
        if (u == T) return flow;
        int sum = 0;
        for (int &i = head[u]; i < (int)(g[u].size()); i++) {
            auto &[v, cap, bak] = g[u][i];
            if (cap && dis[v] == dis[u] + 1) {
                int fl = dfs(v, std::min(flow, cap));
                if (fl) {
                    sum += fl, flow -= fl;
                    cap -= fl, std::get<1>(g[v][bak]) += fl;
                    if (!flow) return sum;
                }
                else dis[v] = -1;
            }
        }
        return sum;
    }
    using EdgeList = std::vector<std::tuple<int,int,int>>;
    EdgeList *g;
    int *dis, *head;
    size_t memsetSz;
    int S, T;
    int maxflow;
};
int main() {
    int n, m, e;
    scanf("%d%d%d", &n, &m, &e);
    MaxFlow mf(n + m + 2);
    int S = n + m + 1, T = n + m + 2;
    for (int i = 1; i <= n; i++) mf.addedg(S, i, 1);
    for (int i = 1; i <= m; i++) mf.addedg(n+i, T, 1);
    for (int u, v; e--; ) {
        scanf("%d%d", &u, &v); v += n;
        mf.addedg(u, v, 1);
    }
    mf.init(S, T);
    printf("%d\n", mf());
    return 0;
}