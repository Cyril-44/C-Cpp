#include <bits/stdc++.h>
constexpr int N = 700005;
#ifdef CLANGD
#define N 75
#endif
std::vector<std::pair<int,int>> g[N], qry[N];
int64_t ans[N], dep[N], tot;
std::vector<int> edgs[N];
int fa[N][2], dfn[N][2];
bool vis[N];
int n;
class SegTr {
    struct Node {
        int64_t hmnadd, add;
        void pull(int64_t hm, int64_t a) {
            hmnadd = std::min(hm + add, hmnadd);
            add += a;
        }
    } tr[N << 2];
    void pushdown(int u) {
        tr[u<<1].pull(tr[u].hmnadd, tr[u].add);
        tr[u<<1|1].pull(tr[u].hmnadd, tr[u].add);
        tr[u].hmnadd = tr[u].add = 0;
    }
    int L, R; int64_t X;
    void upd(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u].pull(X<0?X:0, X);
        int mid = l + r >> 1; pushdown(u);
        if (L <= mid) upd(u<<1, l, mid);
        if (mid < R) upd(u<<1|1, mid+1, r);
    }
    int64_t inq(int u, int l, int r) {
        if (l == r) return tr[u].hmnadd;
        int mid = l + r >> 1; pushdown(u);
        return L <= mid ? inq(u<<1, l, mid) : inq(u<<1|1, mid+1, r);
    }
public:
    void build(int u=1, int l=1, int r=n) {
        if (l == r) return void(tr[u].add = tr[u].hmnadd = tot - dep[l]);
        int mid = l + r >> 1;
        build(u<<1, l, mid), build(u<<1|1, mid+1, r);
    }
    void update(int l, int r, int64_t x) { if (l <= r) L=l, R=r, X=x, upd(1, 1, n); }
    int64_t inquire(int p) { L=p; return inq(1, 1, n); }
} f;
void dfs(int u) {
    dfn[u][0] = ++**dfn;
    for (auto [v, w] : g[u]) if (v != fa[u][0]) {
        fa[v][0] = u, fa[v][1] = w;
        dep[**dfn+1] = dep[dfn[u][0]] + w;
        dfs(v);
    }
    dfn[u][1] = **dfn;
}
int main() {
    freopen("val.in", "r", stdin);
    freopen("val.out", "w", stdout);
    int q, s, k;
    scanf("%d%d%d%d", &n, &q, &s, &k);
    tot = -k * (n+1ll);
    for (int i = 1, u, v, w; i < n; i++) {
        scanf("%d%d%d", &u, &v, &w);
        g[u].emplace_back(v, w);
        g[v].emplace_back(u, w);
        tot += 2*w;
    }
    dfs(s);
    f.build();
    for (int i = 1; i <= q; i++) {
        int t, x; scanf("%d%d", &t, &x);
        x = std::max(x, 1);
        qry[x].emplace_back(t, i);
    }
    vis[s] = true;
    for (int i = 1; i <= n; i++)
        for (int u = i; !vis[u]; u = fa[u][0])
            edgs[i].push_back(u), vis[u] = true;
    for (int i = n; i >= 1; i--) {
        f.update(1, n, k);
        for (int v : edgs[i]) {
            const int u = fa[v][0], w = fa[v][1];
            f.update(1, dfn[v][0] - 1, -2*w);
            f.update(dfn[v][1] + 1, n, -2*w);
        }
        for (auto [t, id] : qry[i])
            ans[id] = f.inquire(dfn[t][0]);
    }
    for (int i = 1; i <= q; i++)
        printf("%lld\n", ans[i]);
}
