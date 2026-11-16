#include <bits/stdc++.h>
constexpr int N = 100001, K = 20;
#ifdef CLANGD
#define N 55
#endif
using Pii = std::pair<int,int>;
std::vector<Pii> g[N];
int dfn[N], mapping[N];
std::set<Pii> subtr[N], qrytr[N];
void dfs1(int u, int fa) { // Get "SonDFN"
    mapping[dfn[u]] = u;
    int id0 = dfn[0];
    for (auto [v,w] : g[u]) if (v != fa) dfn[v] = ++dfn[0];
    int id1 = dfn[0];
    for (auto [v,w] : g[u]) if (v != fa) dfs1(v, u);
    subtr[u] = qrytr[u] = {Pii(dfn[u], dfn[u])};
    if (dfn[0] != id0) {
        if (dfn[u] != id0) subtr[u] = {Pii(dfn[u], dfn[u]), Pii(id0+1, dfn[0])};
        else subtr[u] = {Pii(dfn[u], dfn[0])};
    }
    else subtr[u] = {Pii(dfn[u], dfn[u])};
    if (dfn[0] != id1) qrytr[u] = {Pii(dfn[u], dfn[u]), Pii(id1+1, dfn[0])};
    else qrytr[u] = {Pii(dfn[u], dfn[u])};
}
std::set<Pii> ranges[N];
int sz[N], fa[N], wdep[K][N], dep[N], allSz, curDep, centroid;
bool vis[N];
int n;
int64_t btr[K][N], itr[K][N], wpre[K][N];
void upd(int64_t *a, int p, int64_t x) { for (; p <= n; p += p & -p) a[p] += x; }
int64_t sum(int64_t* a, int p) { int64_t res = 0; for (; p > 0; p -= p & -p) res += a[p]; return res; }
void add(int k, int p, int x) { upd(btr[k], p, x), upd(itr[k], p, x * wpre[k][p-1]); }
int64_t sum(int k, int p) { return sum(btr[k], p) * wpre[k][p] - sum(itr[k], p); }
void rangeAdd(int k, int l, int r, int x) { add(k, l, x), add(k, r+1, -x); }
int64_t rangeSum(int k, int l, int r) { return sum(k, r) - sum(k, l-1); }
void dfs2(int u, int fa) { // Fetch centroid
    sz[u] = 1;
    int mxsz = 0;
    for (auto [v,w] : g[u]) if (v != fa && !vis[v]) {
        dfs2(v, u);
        sz[u] += sz[v];
        mxsz = std::max(mxsz, sz[v]);
    }
    mxsz = std::max(mxsz, allSz - sz[u]);
    if (mxsz <= allSz / 2) centroid = u;
}
void dfs3(int u, int fa) {
    for (auto [v,w] : g[u]) if (v != fa && !vis[v]) {
        wdep[curDep][dfn[v]] = wdep[curDep][dfn[u]] + w;
        dfs3(v, u);
    }
}
#define L first
#define R second
inline std::set<Pii> merge(const std::set<Pii> &x, const std::set<Pii> &y) {
    std::set<Pii> res;
    auto itx = x.begin(), ity = y.begin();
    while (itx != x.end() && ity != y.end()) {
        if (itx->R < ity->L) ++itx;
        else if (ity->R < itx->L) ++ity;
        else if (itx->R < ity->R) res.emplace_hint(res.end(), std::max(itx->L, ity->L), itx->R), ++itx;
        else if (ity->R < itx->R) res.emplace_hint(res.end(), std::max(itx->L, ity->L), ity->R), ++ity;
        else res.emplace_hint(res.end(), std::max(itx->L, ity->L), itx->R), ++itx, ++ity;
    }
    return res;
}
inline std::set<Pii> minus(const std::set<Pii> &x, const std::set<Pii> &y) {
    const auto uni = merge(x, y);
    std::set<Pii> res;
    auto it = uni.begin();
    for (auto [l, r] : x) {
        int last = l;
        for (; it != uni.end() && it->R <= r; ++it) {
            if (last < it->L) res.emplace_hint(res.end(), last, it->L-1);
            last = it->R + 1;
        }
        if (last <= r) res.emplace_hint(res.end(), last, r);
    }
    return res;
}
int dfs4(int u, int s, int d, std::set<Pii> st) { // Build Point Division Tree
    allSz = s, dfs2(u, 0), u = centroid;
    vis[u] = true, ranges[u] = st, dep[u] = d;
    curDep = d, dfs3(u, 0);
    for (auto [v,w] : g[u]) if (!vis[v])
        fa[dfs4(v, sz[u] > sz[v] ? sz[v] : s - sz[u], d+1, dfn[u] < dfn[v] ? merge(st, subtr[v]) : minus(st, subtr[u]))] = u;
    return u;
}

int main() {
    std::cin.tie(nullptr)->sync_with_stdio(false);
    int q; std::cin >> n >> q;
    for (int i = 2; i <= n; i++) {
        int v, w; std::cin >> v >> w;
        g[v].emplace_back(i, w);
        g[i].emplace_back(v, w);
    }
    dfn[1] = dfn[0] = 1, dfs1(1, 0);
    dfs4(1, n, 1, {Pii(1, n)});
    std::iota(wpre[0]+1, wpre[0]+1+n, 1ll);
    for (int k = 1; k < K; k++)
        for (int i = 1; i <= n; i++)
            wpre[k][i] = wpre[k][i-1] + wdep[k][i];
    for (int op, x, y; q--; ) {
        std::cin >> op >> x;
        if (op == 1) {
            std::cin >> y;
            for (int p = y; p; p = fa[p]) {
                for (auto [l, r] : merge(ranges[p], qrytr[x])) {
                    rangeAdd(dep[p], l, r, 1);
                    rangeAdd(0, l, r, wdep[dep[p]][dfn[y]]);
                    if (fa[p]) {
                        rangeAdd(dep[fa[p]], l, r, -1);
                        rangeAdd(0, l, r, -wdep[dep[fa[p]]][dfn[y]]);
                    }
                }
            }
        } else {
            int64_t ans = 0;
            for (auto [l, r] : qrytr[x])
                for (int k = 0; k < K; k++)
                    ans += rangeSum(k, l, r);
            std::cout << ans << '\n';
        }
    }
}
