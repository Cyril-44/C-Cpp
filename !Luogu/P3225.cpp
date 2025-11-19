#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <set>
#include <vector>
constexpr int N = 1005;
struct Edge {
    int to, nxt;
} edgs[N << 1];
int edghead[N];
inline void addedg(int fr, int to) {
    edgs[++edghead[0]] = Edge {to, edghead[fr]};
    edghead[fr] = edghead[0];
}
bool vcut[N];
std::vector<std::vector<int>> dccs;
namespace Tarjan { // 无向图
    unsigned dfn[N], low[N], ts;
    int root;
    int sta[N];
    void dfs(int u, int fr) {
        dfn[u] = low[u] = ++ts;
        sta[++sta[0]] = u;
        int f = 0;
        for (int i = edghead[u], v; i; i = edgs[i].nxt) {
            if ((i-1 ^ fr-1) == 1) continue; // 不从来边更新
            if (!dfn[v = edgs[i].to]) {
                dfs(v, i);
                if (dfn[u] <= low[v]) {
                    if (u != root || ++f >= 2) vcut[u] = true;
                    dccs.emplace_back();
                    do dccs.back().push_back(sta[sta[0]]);
                    while (sta[sta[0]--] != v);
                    dccs.back().push_back(u);
                }
                low[u] = std::min(low[u], low[v]);
            }
            else low[u] = std::min(low[u], dfn[v]);
        }
        if (u == root && !edghead[u]) {
            dccs.push_back({u});
            --sta[0];
        }
    }
    inline void tarj(int n) {
        ts = 0;
        dccs.clear();
        memset(vcut, 0, sizeof vcut);
        memset(dfn, 0, sizeof(int) * (n+1));
        for (int i = 1; i <= n; i++)
            if (!dfn[i]) root = i, dfs(i, 0);
    }
}
int deg[N];
int main() {
#ifndef ONLINE_JUDGE
    freopen("P3225_2.in", "r", stdin);
    freopen("P3225.out", "w", stdout);
#endif
    for (int n, testcase=1; scanf("%d", &n) && n; ++testcase) {
        memset(edghead, 0, sizeof edghead);
        memset(edgs, 0, sizeof edgs);
        int mx = 0;
        for (int u, v; n--; ) {
            scanf("%d%d", &u, &v);
            addedg(u, v);
            addedg(v, u);
            mx = std::max(mx, std::max(u, v));
        }
        n = mx;
        Tarjan::tarj(n);
        int ans1 = 0;
        unsigned long long ans2 = 1;
        for (const auto& dcc : dccs) {
            int cnt = 0;
            for (int i : dcc)
                if (vcut[i]) ++cnt;
            switch (cnt) {
            case 0: // 不可到达别的联通块
                ans1 += 2, ans2 *= (dcc.size() - 1ull) * (dcc.size()) / 2;
                break;
            case 1: // 可以到达别的联通块
                ans1 += 1, ans2 *= dcc.size() - 1ull;
                break;
            default: // 可以到达 1 个以上的联通块
                break;
            }
        }
        if (ans1 < 2) ans1 = 2, ans2 = n * (n - 1ull) / 2;
        printf("Case %d: %d %llu\n", testcase, ans1, ans2);
    }
    return 0;
}
