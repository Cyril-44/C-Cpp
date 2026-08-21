#include <cstdio>
#include <vector>
#include <algorithm>
constexpr int N = 200005, K = 17;
int dep[N], fa[N][K+1], n;
int ans = 0;
inline int lca(int u, int v) {
    if (dep[u] < dep[v]) std::swap(u, v);
    for (int x = dep[u] - dep[v]; x; x &= x-1)
        u = fa[u][__builtin_ctz(x)];
    if (u == v) return u;
    for (int k = K; k >= 0; k--)
        if (fa[u][k] != fa[v][k])
            u = fa[u][k], v = fa[v][k];
    return fa[u][0];
}
inline int _dis(int u, int v, int l) { return dep[u] + dep[v] - 2*dep[l]; }
inline int dis(int u, int v) { return _dis(u, v, lca(u, v)); }
inline int dis(int x, int y, int u) {
    int lxy = lca(x, y), lxu = lca(x, u), lyu = lca(y, u);
    if (lxy == lxu) return dep[u] - dep[lyu];
    else if (lxy == lyu) return dep[u] - dep[lxu];
    else if (lxu == lyu) return _dis(u, lxy, lxu);
}
inline void optim(int &x, int &y, int &d, int &l, int u) {
    int dx = dis(u, x), dy = dis(u, y), mxd = std::max({dx, dy, d});
    if (d == mxd) l = std::max(l, dis(x, y, u));
    else if (dx == mxd) y = u, d = dx;
    else if (dy == mxd) x = u, d = dy;
}
int main() {
    int id;
    scanf("%d%d", &id, &n);
    int x=1, y=1, d=0, l=0; // 维护直径和一条最长第三链
    for (int i = 2; i <= n; i++) {
        scanf("%d", &fa[i][0]);
        fa[i][0] ^= ans;
        // fprintf(stderr, "%d --- %d\n", fa[i][0], i);
        dep[i] = dep[fa[i][0]] + 1;
        for (int k = 1; k <= K; k++)
            fa[i][k] = fa[fa[i][k-1]][k-1];
        optim(x, y, d, l, i);
        // fprintf(stderr, "Diam %d-%d [%d], L=%d\n", x, y, d, l);
        printf("%d\n", ans = d + l - (l>0));
    }
    return 0;
}