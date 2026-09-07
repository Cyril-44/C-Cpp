#include <bits/stdc++.h>
constexpr int N = 1000004;
constexpr int64_t INF = 1e18;
std::vector<std::pair<int,int>> g[N];
int sz[N], hson[N], dep[N], dfn[N], top[N], bot[N], fa[N][2], n;
int64_t presum[N], sufsum[N], f[N];
struct Max { int64_t v = -INF; int id; };
struct Max2 {
    Max mx, mx2;
    Max2& operator+=(const Max& m) {
        if (m.v > mx.v) {
            if (m.id == mx.id) mx.v = m.v;
            else mx2 = mx, mx = m;
        } else if (m.v > mx2.v && m.id != mx.id)
            mx2 = m;
        return *this;
    }
    Max2& operator+=(const Max2& m) {
        *this += m.mx; *this += m.mx2;
        return *this;
    }
    Max2 operator+(Max2 m) const { return m += *this; }
} fmx[N];
template<class T> inline void umax(T &x, T y) { if (x < y) x = y; }
void dfs1(int u) {
    sz[u] = 1;
    for (auto [v,w] : g[u]) if (v != fa[u][0]) {
        dep[v] = dep[u] + 1;
        fa[v][0] = u, fa[v][1] = w;
        dfs1(v);
        sz[u] += sz[v];
        if (sz[v] > sz[hson[u]]) hson[u] = v;
    }
}
class SegTr {
    int64_t tr[N << 2];
    int64_t X; int P, L, R;
    int64_t inq(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u];
        int mid = l + r >> 1;
        if (R <= mid) return inq(u<<1, l, mid);
        if (mid < L) return inq(u<<1|1, mid+1, r);
        return std::max(inq(u<<1, l, mid), inq(u<<1|1, mid+1, r));
    }
    void upd(int u, int l, int r) {
        if (l == r) { umax(tr[u], X); return; }
        int mid = l + r >> 1;
        if (P <= mid) upd(u<<1, l, mid);
        else upd(u<<1|1, mid+1, r);
        tr[u] = std::max(tr[u<<1], tr[u<<1|1]);
    }
public:
    int64_t inquire(int l, int r) { if (!l || !r || l > r) return -INF; L=l, R=r; return inq(1, 1, n); }
    void update(int p, int64_t x) { P=p, X=x; upd(1, 1, n); }
    void build() { std::fill(tr+1, tr+1+n*4, -INF); }
} fpre, fsuf;
void dfs2(int u) {
    dfn[u] = ++dfn[0];
    if (!hson[u]) { bot[u] = u; return; }
    top[hson[u]] = top[u], presum[hson[u]] = presum[u] + fa[hson[u]][1];
    dfs2(hson[u]);
    bot[u] = bot[hson[u]], sufsum[u] = sufsum[hson[u]] + fa[hson[u]][1];
    for (auto [v,w] : g[u]) if (v != fa[u][0] && v != hson[u]) {
        top[v] = v, presum[v] = fa[v][1], dfs2(v);
    }
}
int64_t calc(int p) {
    int64_t sum = 0, res = -INF;
    for (int last = 0; p; last = top[p], p = fa[top[p]][0]) {
        res = std::max(res, sum + std::max({
            fsuf.inquire(dfn[top[p]], dfn[fa[p][0]]) - sufsum[p],
            fpre.inquire(dfn[hson[p]], dfn[bot[p]]) - presum[p],
            last == fmx[p].mx.id ? fmx[p].mx2.v : fmx[p].mx.v
        }));
        sum += presum[p];
    }
    return res;
}
void update(int p) {
    f[p] = p == 1 ? 0 : calc(p);
    fmx[p] += Max{f[p], p};
    for (; p; p = fa[top[p]][0]) {
        fmx[fa[top[p]][0]] += Max{fmx[p].mx.v + presum[p], top[p]};
        fpre.update(dfn[p], fmx[p].mx.v + presum[p]);
        fsuf.update(dfn[p], fmx[p].mx.v + sufsum[p]);
    }
}
int main() {
    scanf("%d", &n);
    for (int i = 1, u, v, x; i < n; i++) {
        scanf("%d%d%d", &u, &v, &x);
        g[u].emplace_back(v, x);
        g[v].emplace_back(u, x);
    }
    dfs1(1), top[1] = 1, dfs2(1), dfn[0] = 0;
    fpre.build(), fsuf.build();
    for (int i = 1; i <= n; i++) update(i);
    printf("%lld\n", f[n]);
    return 0;
}