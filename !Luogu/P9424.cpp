#include <algorithm>
#include <limits>
#include <stdio.h>
#include <vector>
constexpr int N = 200005;
int w[N];
struct Edge {
    int to, nxt;
} e[N << 1];
int head[N]{1};
inline void addedg(int fr, int to) {
    e[++head[0]] = Edge{to, head[fr]};
    head[fr] = head[0];
}
bool ecut[N << 1];
long long bcc[N];
int bl2[N];
int onecnt = 0;
namespace Tarjan {
unsigned dfn[N], low[N], ts;
void dfs(int u, int fr) {
    dfn[u] = low[u] = ++ts;
    for (int i = head[u], v; i; i = e[i].nxt) {
        if ((i ^ fr) == 1) continue;
        v = e[i].to;
        if (!dfn[v]) {
            dfs(v, i);
            low[u] = std::min(low[u], low[v]);
            if (dfn[u] < low[v]) ecut[i] = ecut[i ^ 1] = true;
        } else
            low[u] = std::min(low[u], dfn[v]);
    }
}

void dfs1(int u) {
    bl2[u] = bcc[0];
    bcc[bcc[0]] += w[u];
    for (int i = head[u], v; i; i = e[i].nxt) {
        if (ecut[i] || bl2[v = e[i].to]) continue;
        dfs1(v);
    }
}
std::vector<int> g[N];
long long size[N];
void dfs2(int u, int fa) {
    size[u] = bcc[u];
    for (int v : g[u]) {
        if (v == fa) continue;
        dfs2(v, u);
        size[u] += size[v];
    }
}
bool vis3[N];
long long www3 = 0;
void dfs3(int u) {
    if (vis3[u]) return;
    www3 += w[u];
    vis3[u] = true;
    for (int i = head[u]; i; i = e[i].nxt)
        dfs3(e[i].to);
}
inline void solve(int n, int m) {
    dfs(1, 0);
    for (int i = 1; i <= n; i++) {
        if (!dfn[i]) {
            dfs(i, 0);
            for (int j = 1; j <= n; j++)
                if (!dfn[i]) return puts("-1"), void();
            if (m > n - 2) {
                dfs3(1); long long www = www3;
                www3 = 0, dfs3(i);
                // fprintf(stderr, "%d %lld, %d %lld\n", 1, bcc[1], i, bcc[2]);
                printf("%lld\n", std::abs(www - www3));
            } else puts("-1");
            return;
        }
    }
    for (int i = 1; i <= n; i++)
        if (!bl2[i]) {
            ++bcc[0];
            dfs1(i);
        }
    if (bcc[0] == 1)
        puts("-1");
    else {
        for (int u = 1; u <= n; u++)
            for (int i = head[u], v; i; i = e[i].nxt) {
                if (bl2[u] ^ bl2[v = e[i].to]) {
                    // fprintf(stderr, "%d->%d %d->%d\n", u, v, bl2[u], bl2[v]);
                    g[bl2[u]].push_back(bl2[v]);
                }
            }
        for (int i = 1; i <= bcc[0]; i++) {
            std::sort(g[i].begin(), g[i].end());
            auto it = std::unique(g[i].begin(), g[i].end());
            while (g[i].end() != it) g[i].pop_back();
        }
        dfs2(1, 0);
        long long sum = 0;
        for (int i = 1; i <= n; i++) sum += w[i];
        long long ans = std::numeric_limits<long long>::max();
        for (int i = 2; i <= bcc[0]; i++) {
            // printf("%lld %lld\n", size[i], sum - size[i]);
            ans = std::min(ans, std::abs(size[i] - (sum - size[i])));
        }
        printf("%lld\n", ans);
    }
}
} // namespace Tarjan
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++) scanf("%d", &w[i]);
    int _m = m;
    for (int u, v; m--;) {
        scanf("%d%d", &u, &v);
        if (u == v) {
            ++onecnt;
            continue;
        }
        addedg(u, v);
        addedg(v, u);
    }
    Tarjan::solve(n, _m);
    return 0;
}