#include <cmath>
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <tuple>
constexpr int N = 155, M = 6005;
int match[N];
struct MaxCostMaxFlow {
    using EdgeList = std::vector<std::tuple<int,int,int,int>>;
    EdgeList *g;
    MaxCostMaxFlow(int n) : g(new EdgeList[n+1]), dis(new long long[n+1]), head(new int[n+1]), vis(new bool[n+1]()), memsetSz(sizeof(int) * (n+1)), S(), T(), maxflow(), maxcost() {}
    inline void init(int s, int t) { S = s, T = t; }
    std::pair<int,long long> operator()() {
        if (!S || !T) return {-1,-1};
        if (maxflow) return {maxflow, maxcost};
        while (spfa()) maxflow += dfs(S, 0x7fffffff);
        return {maxflow, maxcost};
    }
    inline void addedg(int fr, int to, int cap, int cost) {
        g[fr].emplace_back(to, cap, cost, (int)g[to].size());
        g[to].emplace_back(fr, 0, -cost, (int)g[fr].size() - 1);
    }
private:
    inline bool spfa() {
        memset(dis, 0x80, memsetSz << 1);
        memset(head, 0, memsetSz);
        static int que[N * M];
        int l = 0, r = 0;
        que[r++] = S, dis[S] = 0;
        while (l < r) {
            int u = que[l++];
            vis[u] = false;
            for (const auto &[v, cap, cost, bak] : g[u])
                if (cap && dis[v] < dis[u] + cost) {
                    dis[v] = dis[u] + cost;
                    if (!vis[v]) vis[v] = true, que[r++] = v;
                }
        }
        return dis[T] != (long long)0x8080808080808080;
    }
    int dfs(int u, int flow) {
        if (u == T) return flow;
        vis[u] = true;
        int sum = 0;
        for (int &i = head[u]; i < (int)g[u].size(); i++) {
            auto &[v, cap, cost, bak] = g[u][i];
            if (cap && !vis[v] && dis[v] == dis[u] + cost) {
                int fl = dfs(v, std::min(flow, cap));
                if (fl) {
                    flow -= fl, sum += fl;
                    cap -= fl, std::get<1>(g[v][bak]) += fl;
                    maxcost += 1ll * fl * cost;
                }
            }
        }
        vis[u] = false;
        return sum;
    }
    long long *dis;
    int *head;
    bool *vis;
    size_t memsetSz;
    int S, T;
    int maxflow;
    long long maxcost;
};
int main() {
    int n, m, e;
    scanf("%d%d%d", &n, &m, &e);
    MaxCostMaxFlow mcmf(n + m + 2);
    int S = n + m + 1, T = n + m + 2;
    mcmf.init(S, T);
    for (int i = 1; i <= n; i++)
        mcmf.addedg(S, i, 1, 0);
    for (int i = 1; i <= m; i++)
        mcmf.addedg(n + i, T, 1, 0);
    for (int u, v, w; e--; ) {
        scanf("%d%d%d", &u, &v, &w);
        mcmf.addedg(u, n + v, 1, w);
    }
    for (int i = 1; i <= n; i++)
        mcmf.addedg(i, T, 1, 0); // 确保最后总能匹配到 n 个，但是有的就是假的匹配
    printf("%lld\n", mcmf().second); // 其次是权最大
    for (int i = 1; i <= n; i++) {
        int res = 0;
        for (const auto &[j, cap, _, __] : mcmf.g[i])
            if (n < j && j <= n+m && !cap) res = j - n; // 用过了就是匹配上了
        printf("%d ", res);
    }
    putchar('\n');
    return 0;
}