#include <stdio.h>
#include <vector>
#include <numeric>
#include <algorithm>
constexpr int N = 100005, K = 17;
int fa[N][K + 1], dep[N], size[N];
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
int main() {
    int n, m;
    scanf("%d", &n);
    for (int i = 1, u, v; i < n; i++) {
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    scanf("%d", &m);
    dfs(1, 0);
    for (int a, b; m--; ) {
        scanf("%d%d", &a, &b);
        if (a == b) { printf("%d\n", n); continue; }
        int d = dis(a, b);
        if (d & 1) { puts("0"); continue; }
        d >>= 1;
        if (dep[a] == dep[b]) {
            int u = jmp(a, d);
            int a1 = jmp(a, d-1);
            int b1 = jmp(b, d-1);
            printf("%d\n", n - size[u] + size[u] - size[a1] - size[b1]);
        } else {
            if (dep[a] < dep[b]) std::swap(a, b);
            int u = jmp(a, d);
            int a1 = jmp(a, d-1);
            printf("%d\n", size[u] - size[a1]);
        }
    }
    return 0;
}