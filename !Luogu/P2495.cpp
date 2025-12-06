#include <limits>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include <set>
constexpr int N = 250005, K = 18;
int a[N];
std::vector<std::pair<int,int>> g[N];
int fa[N][K+1], mn[N][K+1], dep[N], dfn[N];
bool key[N];
int n;
namespace Init {
int ts = 0;
void dfs(int u) {
    dfn[u] = ++ts;
    for (int k = 1; k <= K; k++) {
        fa[u][k] = fa[fa[u][k-1]][k-1];
        mn[u][k] = std::min(mn[u][k-1], mn[fa[u][k-1]][k-1]);
    }
    for (const auto&[v, w] : g[u]) {
        if (v == fa[u][0]) continue;
        fa[v][0] = u;
        mn[v][0] = w;
        dep[v] = dep[u] + 1;
        dfs(v);
    }
}
} // namespace Init
int lca(int u, int v) {
    if (dep[u] < dep[v]) std::swap(u, v);
    for (int tp = dep[u] - dep[v]; tp; tp &= tp - 1)
        u = fa[u][__builtin_ctz(tp)];
    if (u == v) return u;
    for (int k = K; k >= 0; k--)
        if (fa[u][k] ^ fa[v][k])
            u = fa[u][k], v = fa[v][k];
    return fa[u][0];
}
int getmn(int u, int v) {
    if (dep[u] < dep[v]) std::swap(u, v);
    int res = std::numeric_limits<int>::max();
    for (int l = dep[u] - dep[v], tp; l; l &= l - 1) {
        res = std::min(res, mn[u][tp = __builtin_ctz(l)]);
        u = fa[u][tp];
    }
    return res;
}
std::vector<int> ng[N];
namespace VT {
int sta[N];
inline void build(std::vector<int>& pts) {
    std::sort(pts.begin(), pts.end(), [](int x, int y){return dfn[x] < dfn[y];});
    int top = 1;
    sta[1] = 1;
    for (int v : pts) {
        int u = lca(v, sta[top]); // lca(v, sta[top])
        if (u != sta[top]) {
            for (; top > 1 && dfn[sta[top-1]] > dfn[u]; --top) // 注意 >=，一直搞到 栈次大 <= dfn[u]
                ng[sta[top-1]].push_back(sta[top]);
            if (sta[top-1] == u) { // u -> (sta[top], v)
                ng[u].push_back(sta[top--]);
            } else { // sta[top-1] -> u -> (sta[top], v)
                ng[u].push_back(sta[top]);
                sta[top] = u;
            }
            // if (sta[top] != u) // lca 不在栈顶，说明栈顶 dep < lca dep，将 lca 接在栈顶下面 
            //     sta[++top] = u;
        }
        sta[++top] = v;
    }
    for (int i = 1; i < top; i++)
        ng[sta[i]].push_back(sta[i+1]);
}
}
namespace Solve {
long long f[N][2];
void dfs(int u) {
    f[u][0] = f[u][1] = 0;
    if (key[u]) {
        f[u][0] = 1ll << 60;
        for (int v : ng[u]) {
            dfs(v);
            f[u][1] += std::min(f[v][0], f[v][1]);
        }
    }
    else 
        for (int v : ng[u]) {
            dfs(v);
            f[u][0] += std::min(f[v][0], f[v][1] + getmn(u, v));
            f[u][1] += f[v][1];
        }
    key[u] = false;
    ng[u].clear();
}
}
int main() {
    scanf("%d", &n);
    for (int i = 1, u, v, w; i < n; i++) {
        scanf("%d%d%d", &u, &v, &w);
        g[u].emplace_back(v, w);
        g[v].emplace_back(u, w);
    }
    Init::dfs(1);
    int m;
    scanf("%d", &m);
    for (int k; m--; ) {
        scanf("%d", &k);
        std::vector<int> a;
        a.reserve(k);
        for (int x; k--; ) {
            scanf("%d", &x);
            a.emplace_back(x);
            key[x] = true;
        }
        VT::build(a);
        // for (int i = 1; i <= n; i++)
        //     for (int j : ng[i])
        //         fprintf(stderr, "%d %d\n", i, j);
        Solve::dfs(1);
        printf("%lld\n", Solve::f[1][0]);
    }
    return 0;
}