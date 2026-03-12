#include <cstdio>
#include <cstring>
#include <algorithm>
constexpr int N = 100005, IntMax = 0x3f3f3f3f;
int a[N];
std::vector<int> edgs[N];
struct Matrix {
    int a[2][2]{{0, -IntMax}, {-IntMax, 0}};
    int* operator[](bool x) { return a[x]; }
    const int* operator[](bool x) const { return a[x]; }
    inline int operator()(bool x) const { return a[x][0]; }
};
inline Matrix operator*(const Matrix &a, const Matrix &b) {
    Matrix ret{{{-IntMax, -IntMax}, {-IntMax, -IntMax}}};
    for (int k = 0; k ^ 2; k++)
        for (int i = 0; i ^ 2; i++)
            for (int j = 0; j ^ 2; j++)
                ret[i][j] = std::max(ret[i][j], a[i][k] + b[k][j]);
    return ret;
}

Matrix f[N], prod[N]; int g[N][2];
int dfn[N], pre[N], size[N], chain[N], chainPreSum[N], hson[N];
int fa[N], ls[N], rs[N];
inline void syncTrans(int u) { // g[u] --> f[u]
    f[u] = {{{g[u][0], g[u][0]}, {g[u][1], -IntMax}}};
}
void dfs(int u) {
    static int f[N][2]{}, timestamp = 0;
    size[u] = 1;
    dfn[u] = ++timestamp;
    for (int v : edgs[u]) {
        if (v == pre[u]) continue;
        pre[v] = u;
        dfs(v);
        size[u] += size[v];
        if (size[v] > size[hson[u]]) hson[u] = v;
    }
    g[u][1] = a[u];
    for (int v : edgs[u]) {
        if (v == pre[u] || v == hson[u]) continue;
        g[u][0] += std::max(f[u][0], f[u][1]);
        g[u][1] += f[u][0];
    }
    f[u][0] = g[u][0] + std::max(f[hson[u]][0], f[hson[u]][1]);
    f[u][1] = g[u][1] + f[hson[u]][0];
}
inline void pushup(int u) {
    prod[u] = prod[ls[u]] * f[u] * prod[rs[u]];
}
int chainBuild(int l, int r) {
    int mid = std::lower_bound(chainPreSum+l, chainPreSum+r+1, (chainPreSum[l-1] + chainPreSum[r]) / 2) - chainPreSum;
    int u = chain[mid]; // 每次选取加权中位数做根
    syncTrans(u);
    if (l ^ mid) ls[u] = chainBuild(l, mid-1), fa[ls[u]] = u;
    if (r ^ mid) rs[u] = chainBuild(mid+1, r), fa[rs[u]] = u;
    pushup(u);
    return u;
}
int globBSTBuild(int u) {
    int v = u;
    do  for (int w : edgs[v])
            if (w != pre[v] && w != hson[v])
                fa[globBSTBuild(w)] = v; // 认父不认子
    while (v = hson[v]);
    chain[0] = 0, v = u;
    do chain[++chain[0]] = v;
    while (v = hson[v]);
    for (int i = 1; i <= chain[0]; i++)
        chainPreSum[i] = chainPreSum[i-1] + size[chain[i]] - size[hson[chain[i]]];
    return chainBuild(1, chain[0]);
}
inline void modify(int u, int x) { // a[u] <-- x
    g[u][1] -= a[u], g[u][1] += a[u] = x;
    syncTrans(u);
    for (; u; u = fa[u])
        if (fa[u] && (ls[fa[u]] != u && rs[fa[u]] != u)) { // 跳到了轻边
            g[fa[u]][0] -= std::max(f[u](0), f[u](1)), g[fa[u]][1] -= f[u](0); syncTrans(fa[u]);
            pushup(u);
            g[fa[u]][0] += std::max(f[u](0), f[u](1)), g[fa[u]][1] += f[u](1); syncTrans(fa[u]);
        }
        else pushup(u);
}

int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
    for (int u, v, i = 1; i ^ n; i++) {
        scanf("%d%d", &u, &v);
        edgs[u].push_back(v);
        edgs[v].push_back(u);
    }
    dfs(1);
    int root = globBSTBuild(1);
    printf("%d\n", std::max(f[root](0), f[root](1)));
    for (int u, x; m--; ) {
        scanf("%d%d", &u, &x);
        modify(u, x);
        printf("%d\n", std::max(f[root](0), f[root](1)));
    }
    return 0;
}