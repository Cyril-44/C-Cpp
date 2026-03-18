#include <cstdio>
#include <cstring>
#include <functional>
#include <vector>
#include <algorithm>
#include <unordered_set>
constexpr int N = 100005;
using LL = long long;
constexpr LL INF = 1e11;
int p[N];
std::vector<int> edgs[N];
struct Matrix {
    inline LL* operator[](int x) { return a[x]; }
    inline const LL* operator[](int x) const { return a[x]; }
    inline LL operator()(int x) const { return ((*this) * Matrix{0,0,0,0})[x][0]; }
    friend Matrix operator*(const Matrix&, const Matrix&);
    LL a[2][2]{{0ll, INF}, {INF, 0ll}};
};
inline Matrix operator*(const Matrix &x, const Matrix &y) {
    Matrix z{{{INF, INF}, {INF, INF}}};
    for (int k = 0; k ^ 2; k++)
        for (int i = 0; i ^ 2; i++)
            for (int j = 0; j ^ 2; j++)
                z[i][j] = std::min(z[i][j], x[i][k] + y[k][j]);
    return z;
}
struct GlobalBST {
    int *chain, *chainPreSum;
    std::vector<int> pre, size, hson, ls, rs, fa;
    std::vector<std::array<LL,2>> f, g;
    std::vector<Matrix> tr, prod;
    void dfs(int u) {
        size[u] = 1;
        for (int v : edgs[u]) {
            if (v == pre[u]) continue;
            pre[v] = u;
            dfs(v);
            size[u] += size[v];
            if (size[v] > size[hson[u]]) hson[u] = v;
        }
        g[u][1] = p[u];
        for (int v : edgs[u]) {
            if (v == pre[u] || v == hson[u]) continue;
            g[u][0] += f[v][1];
            g[u][1] += std::min(f[v][0], f[v][1]);
        }
        f[u][0] = g[u][0] + f[hson[u]][1];
        f[u][1] = g[u][1] + std::min(f[hson[u]][0], f[hson[u]][1]);
    }
    inline void syncTrans(int u) { tr[u] = {{{INF, g[u][0]}, {g[u][1], g[u][1]}}}; }
    inline void pushup(int u) { prod[u] = prod[ls[u]] * tr[u] * prod[rs[u]]; }
    int chainBuild(int l, int r) {
        int mid = std::lower_bound(chainPreSum+l, chainPreSum+r+1, chainPreSum[l-1] + chainPreSum[r] >> 1) - chainPreSum;
        int u = chain[mid];
        if (l ^ mid) ls[u] = chainBuild(l, mid-1), fa[ls[u]] = u;
        if (r ^ mid) rs[u] = chainBuild(mid+1, r), fa[rs[u]] = u;
        syncTrans(u); pushup(u);
        return u;
    }
    int build(int u) {
        int v = u;
        do  for (int w : edgs[v])
                if (w != pre[v] && w != hson[v])
                    fa[build(w)] = v;
        while (v = hson[v]);
        v = u, chain[0] = 0;
        do chain[++chain[0]] = v;
        while (v = hson[v]);
        for (int i = 1; i <= chain[0]; i++)
            chainPreSum[i] = chainPreSum[i-1] + size[chain[i]] - size[hson[chain[i]]];
        return chainBuild(1, chain[0]);
    }
    inline void update(int u) {
        for (; u; u = fa[u])
            if (fa[u] && ls[fa[u]] != u && rs[fa[u]] != u) {
                g[fa[u]][0] -= prod[u](1), g[fa[u]][1] -= std::min(prod[u](0), prod[u](1));
                pushup(u);
                g[fa[u]][0] += prod[u](1), g[fa[u]][1] += std::min(prod[u](0), prod[u](1));
                syncTrans(fa[u]);
            }
            else pushup(u);
    }
    inline LL modifyAndQuery(int a, bool x, int b, bool y) {
        LL tmpa = g[a][!x]; LL tmpb = g[b][!y];
        g[a][!x] = INF; syncTrans(a); update(a);
        g[b][!y] = INF; syncTrans(b); update(b);
        LL ret = std::min(prod[root](0), prod[root](1));
        g[a][!x] = tmpa, syncTrans(a), update(a);
        g[b][!y] = tmpb, syncTrans(b), update(b);
        return ret;
    }
    int root;
    GlobalBST(int n, int rt=1) : 
        chain(new int[n+1]), chainPreSum(new int[n+1]),
        pre(n+1), size(n+1), hson(n+1), ls(n+1), rs(n+1), fa(n+1), 
        f(n+1), g(n+1),
        tr(n+1), prod(n+1)
        { *chainPreSum = 0; dfs(rt); root = build(rt); }
};
template<>
struct std::hash<std::pair<int,int>> {
    inline std::uint64_t operator()(const std::pair<int,int> &p) const {
        return (std::uint64_t)p.first << 32 | p.second;
    }
};
int main() {
    int n, m;
    scanf("%d%d %*s", &n, &m);
    for (int i = 1; i <= n; i++)
        scanf("%d", &p[i]);
    std::unordered_set<std::pair<int,int>> alledgs;
    for (int i = 1, u, v; i < n; i++) {
        scanf("%d%d", &u, &v);
        edgs[u].push_back(v);
        edgs[v].push_back(u);
        alledgs.emplace(u, v);
        alledgs.emplace(v, u);
    }
    GlobalBST ddp(n);
    for (int a, x, b, y; m--; ) {
        scanf("%d%d%d%d", &a, &x, &b, &y);
        if (!x && !y && alledgs.count({a, b}) || a == b && x != y) puts("-1");
        else printf("%lld\n", ddp.modifyAndQuery(a, x, b, y));
    }
    return 0;
}