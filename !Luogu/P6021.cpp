#include <cassert>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
constexpr int N = 200005;
using LL = long long;
constexpr LL INF = 0x3f3f3f3f3f3f3f3fll;
int a[N];
std::vector<int> edgs[N];
struct Matrix {
    LL a[2][2]{{0, INF}, {INF, 0}};
    inline LL* operator[](int x) { return a[x]; }
    inline const LL* operator[](int x) const { return a[x]; }
    inline LL operator()() const { return std::min(a[0][0], a[0][1]); } // Equals to (*this * Mat{0,0,0,0})[0][0]
};
inline Matrix operator*(const Matrix &x, const Matrix &y) {
    Matrix z; memset(z.a, 0x3f, sizeof z.a);
    for (int k = 0; k ^ 2; k++)
        for (int i = 0; i ^ 2; i++)
            for (int j = 0; j ^ 2; j++)
                z[i][j] = std::min(z[i][j], x[i][k] + y[k][j]);
    return z;
}
struct GlobalBST {
    int *chain, *chainPreSum;
    std::vector<int> pre, size, hson, fa, ls, rs;
    std::vector<LL> f, g;
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
        g[u] = 0;
        for (int v : edgs[u]) {
            if (v == pre[u] || v == hson[u]) continue;
            g[u] += f[v];
        }
        if (!hson[u]) g[u] = INF, f[u] = a[u];
        else f[u] = std::min((LL)a[u], g[u] + f[hson[u]]);
    }
    inline void syncTrans(int u) {
        tr[u] = {{{g[u], a[u]}, {INF,0}}};
    }
    inline void pushup(int u) {
        prod[u] = prod[ls[u]] * tr[u] * prod[rs[u]];
    }
    int chainBuild(int l, int r) {
        int mid = std::lower_bound(chainPreSum+l, chainPreSum+r+1, (chainPreSum[l-1] + chainPreSum[r]) / 2) - chainPreSum;
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
        chain[0] = 0; v = u;
        do chain[++chain[0]] = v;
        while (v = hson[v]);
        for (int i = 1; i <= chain[0]; i++)
            chainPreSum[i] = chainPreSum[i-1] + size[chain[i]] - size[hson[chain[i]]];
        return chainBuild(1, chain[0]);
    }
    inline void modify(int u, int x) {
        a[u] += x; syncTrans(u);
        for (; u; u = fa[u])
            if (fa[u] && ls[fa[u]] != u && rs[fa[u]] != u) {
                g[fa[u]] -= prod[u]();
                pushup(u);
                g[fa[u]] += prod[u]();
                syncTrans(fa[u]);
            }
            else pushup(u);
    }
    inline LL query(int u) {
        Matrix ret;
        bool flg = true;
        for (; u; u = fa[u]) {
            if (flg) ret = (ret * tr[u]) * prod[rs[u]];
            flg = ls[fa[u]] == u; // 只统计往右上跳
            if (!flg && rs[fa[u]] != u) break; // 到根了
        }
        return ret();
    }
    int root;
    GlobalBST(int n, int rt=1) : chain(new int[n+1]), chainPreSum(new int[n+1]()), 
        pre(n+1), size(n+1), hson(n+1), fa(n+1), ls(n+1), rs(n+1), f(n+1), g(n+1), tr(n+1), prod(n+1) { dfs(rt); root = build(rt); }
    ~GlobalBST() { delete[] chain; delete[] chainPreSum; }
};
int main() {
    int n, m;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
    for (int u, v, i = 1; i ^ n; i++) {
        scanf("%d%d", &u, &v);
        edgs[u].push_back(v);
        edgs[v].push_back(u);
    }
    scanf("%d", &m);
    GlobalBST ddp(n);
    char ch;
    for (int x, t; m--; ) {
        scanf(" %c%d", &ch, &x);
        if (ch == 'C') {
            scanf("%d", &t);
            ddp.modify(x, t);
        }
        else printf("%lld\n", ddp.query(x));
    }
    return 0;
}
