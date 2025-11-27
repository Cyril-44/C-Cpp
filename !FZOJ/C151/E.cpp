#include <stdio.h>
#include <vector>
#include <numeric>
#include <algorithm>
constexpr int N = 300005, K = 19;
int fa[N][K + 1], dep[N], size[N], w[N], arr[N];
std::vector<int> g[N];
void dfs(int u, int pre) {
    fa[u][0] = pre;
    size[u] = 1;
    for (int k = 1; k <= K; k++)
        fa[u][k] = fa[fa[u][k-1]][k-1];
    for (int v : g[u]) {
        if (v ^ pre) {
            dep[v] = dep[u] + 1;
            dfs(v, u);
            size[u] += size[v];
        }
    }
}
inline int lca(int u, int v) {
    if (dep[u] < dep[v]) std::swap(u, v);
    for (int tp = dep[u] - dep[v], k = K; k >= 0; k--)
        if (tp >> k & 1) u = fa[u][k];
    if (u == v) return u;
    for (int k = K; k >= 0; k--)
        if (fa[u][k] ^ fa[v][k])
            u = fa[u][k], v = fa[v][k];
    return fa[u][0];
}
inline int dis(int u, int v) {
    int com = lca(u, v);
    return dep[u] - dep[com] + dep[v] - dep[com];
}
int jmp(int u, int n) {
    while (n) {
        u = fa[u][__builtin_ctz(n)];
        n &= n - 1;
    }
    return u;
}
void solve(int u, int pre) {
    for (int v : g[u]) {
        if (v ^ pre) {
            solve(v, u);
            w[u] += w[v];
        }
    }
}
inline void add(int a, int b) {
    int com = lca(a, b);
    if (dep[a] > dep[b]) std::swap(a, b);
    ++w[a], ++w[b];
    w[com] -= 2;
}
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int i = 1, u, v; i < n; i++) {
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    dfs(1, 0);
    for (int i = 1, u, v; i <= m; i++) {
        scanf("%d%d", &u, &v);
        add(u, v);
    }
    solve(1, 0);
    long long ans = 0;
    for (int i = 2; i <= n; i++)
        if (w[i] == 1) ++ans;
        else if (!w[i]) ans += m;
    printf("%lld\n", ans);
    return 0;
}