#include <limits>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <tuple>
constexpr int N = 100005;
using LL = long long;
struct MaxFlow {
    MaxFlow(int n, int s = 0, int t = 0) : g(new EdgeList[n+1]), head(new int[n+1]), dis(new int[n+1]), n(n), S(s), T(t), maxflow(0) {}
    ~MaxFlow() { delete[] g; delete[] dis; delete[] head; }
    inline void init(int s, int t) { S = s, T = t; }
    inline LL operator()() {
        if (maxflow) return maxflow;
        do {
            memset(dis, 0, sizeof(int) * (n+1));
            memset(head, 0, sizeof(int) * (n+1));
            if (!bfs()) break;
            maxflow += dfs(S, std::numeric_limits<int>::max());
        } while (true);
        return maxflow;
    }
    inline void addedg(int u, int v, int cap) {
        g[u].emplace_back(v, cap, (int)g[v].size());
        g[v].emplace_back(u, 0, (int)g[u].size() - 1);
    }
private:
    inline bool bfs() {
        static int que[N];
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
    LL dfs(int u, int flow) {
        if (u == T) return flow; // 找到汇点返回当前流量！
        LL sum = 0;
        for (int &i = head[u]; i < (int)g[u].size(); ++i) { // 当前弧优化
            auto &[v, cap, bak] = g[u][i];
            if (cap && dis[v] == dis[u] + 1) {
                LL fl = dfs(v, std::min(flow, cap));
                if (fl) { // 增流
                    cap -= fl;
                    std::get<1>(g[v][bak]) += fl;
                    sum += fl, flow -= fl;
                    if (!flow) return sum;
                }
                else dis[v] = -1; // 点优化
            }
        }
        return sum;
    }
    using EdgeList = std::vector<std::tuple<int,int,int>>;
    EdgeList *g;
    int *head, *dis;
    LL maxflow;
    int n, S, T;
};
int main() {
    int n, m, s, t;
    scanf("%d%d%d%d", &n, &m, &s, &t);
    MaxFlow mf(n, s, t);
    for (int u, v, w, i=m; i--;) {
        scanf("%d%d%d", &u, &v, &w);
        mf.addedg(u, v, w);
    }
    printf("%lld\n", mf());
    return 0;
}