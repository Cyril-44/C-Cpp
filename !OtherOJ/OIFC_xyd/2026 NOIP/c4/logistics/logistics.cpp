#include <bits/stdc++.h>
constexpr int N = 100005, K = 17;
constexpr int64_t INF = 1e18;
#ifdef CLANGD
#define N 15
#endif
std::tuple<int,int,int> edgs[N];
int64_t a[N], f[N];
int b[N], head[N], tail[N], to[N << 1], tow[N << 1];
namespace LCA {
    int st[N][K+1], dfn[N];
    int64_t dep[N];
    void dfs(int u, int fa) {
        dfn[u] = ++dfn[0];
        st[dfn[u]][0] = fa;
        for (int i = head[u]; i <= tail[u]; i++) {
            const int v = to[i];
            if (v == fa) continue;
            dep[v] = dep[u] + tow[i];
            dfs(v, u);
        }
    }
    inline int min(int u, int v) { return dfn[u] < dfn[v] ? u : v; }
    inline void init(const int n) {
        dfs(1, 0);
        for (int k = 1; k <= K; k++)
            for (int i = 1; i+(1<<k)-1 <= n; i++)
                st[i][k] = min(st[i][k-1], st[i + (1<<k-1)][k-1]);
    }
    inline int lca(int u, int v) {
        if (u == v) return u;
        u = dfn[u], v = dfn[v];
        if (u > v) std::swap(u, v);
        const int k = 31 - __builtin_clz(v - u++);
        return min(st[u][k], st[v - (1<<k) + 1][k]);
    }
    inline int64_t dis(int u, int v) {
        return dep[u] + dep[v] - 2 * dep[lca(u, v)];
    }
}
class LCSeg {
    struct Line {
        int64_t b;
        int k;
        Line(int k=0, int64_t b=INF) : b(b), k(k) {}
        int64_t operator()(int64_t x) const { return k * x + b; }
    } tr[N * K * 2];
    int ls[N * K * 2], rs[N * K * 2], rt[N], top;
    void upd(int &u, int64_t l, int64_t r, Line x) {
        if (!u) u = ++top;
        const int64_t mid = l + r >> 1;
        if (x(mid) < tr[u](mid)) std::swap(tr[u], x);
        if (l == r) return;
        if (x(l) < tr[u](l)) upd(ls[u], l, mid, x);
        if (x(r) < tr[u](r)) upd(rs[u], mid+1, r, x);
    }
    int64_t P;
    int64_t inq(int u, int64_t l, int64_t r) {
        if (!u) return INF;
        int64_t res = tr[u](P);
        if (l != r) {
            const int64_t mid = l + r >> 1;
            res = std::min(res, P <= mid ? inq(ls[u], l, mid) : inq(rs[u], mid+1, r));
        }
        return res;
    }
public:
    void update(int i, Line x) { upd(rt[i], 0, (int64_t)1e11, x); }
    int64_t inquire(int i, int64_t x) { P=x; return inq(rt[i], 0, (int64_t)1e11); }
} flc;
namespace PntDvdTr {
    bool vis[N];
    int sz[N], fa[N], s, centroid;
    void dfs1(int u, int fa) {
        sz[u] = 1;
        int mxsub = 0;
        for (int i = head[u]; i <= tail[u]; i++) {
            const int v = to[i];
            if (vis[v] || v == fa) continue;
            dfs1(v, u);
            sz[u] += sz[v];
            mxsub = std::max(mxsub, sz[v]);
        }
        mxsub = std::max(mxsub, s - sz[u]);
        if (mxsub <= s / 2) centroid = u;
    }
    int dfs(int u, int us) {
        s = us, dfs1(u, 0), vis[u = centroid] = true;
        for (int i = head[u]; i <= tail[u]; i++) {
            const int v = to[i];
            if (vis[v]) continue;
            fa[dfs(v, sz[v] < sz[u] ? sz[v] : us - sz[u])] = u;
        }
        return u;
    }
    inline void inquire(int u) {
        for (int c = u; c; c = fa[c])
            f[u] = std::min(f[u], flc.inquire(c, LCA::dis(c, u)));
    }
    inline void insert(int u) { // k = b[u], b = f[u] + a[u] + dis(u, Centre)*b[u]
        // fprintf(stderr, "Updating with node %d:\n", u);
        for (int c = u; c; c = fa[c]) {
            flc.update(c, {b[u], f[u] + a[u] + LCA::dis(u, c) * b[u]});
            // fprintf(stderr, "Inserting line (%d, %lld) to dividing centre %d\n", b[u], f[u] + a[u] + (int64_t)LCA::dis(u, c) * b[u], c);
        }
    }
}

int main() {
    freopen("logistics.in", "r", stdin);
    freopen("logistics.out", "w", stdout);
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) scanf("%lld", &a[i]);
    for (int i = 1; i <= n; i++) scanf("%d", &b[i]);
    std::vector<std::tuple<int,int,int>> edgs(n-1);
    for (auto &[u, v, w] : edgs) {
        scanf("%d%d%d", &u, &v, &w);
        ++tail[++u], ++tail[++v];
    }
    for (int i = 1; i <= n; i++) head[i] = (tail[i] += tail[i-1]) + 1;
    for (const auto &[u, v, w] : edgs){
        to[--head[u]] = v; tow[head[u]] = w;
        to[--head[v]] = u; tow[head[v]] = w;
    }
    LCA::init(n);
    int rt = PntDvdTr::dfs(1, n);
    // for (int i = 1; i <= n; i++) fprintf(stderr, "on PntDvdTree, fa[%d]=%d\n", i, PntDvdTr::fa[i]);
    std::priority_queue<std::pair<int,int>> pq;
    memset(f, 0x3f, sizeof f);
    f[1] = 0; PntDvdTr::insert(1);
    for (int i = 2; i <= n; i++)
        if (b[i] <= b[1]) pq.emplace(b[i], i);
    while (!pq.empty()) {
        auto [_, u] = pq.top(); pq.pop();
        PntDvdTr::inquire(u);
        PntDvdTr::insert(u);
    }
    for (int i = 2; i <= n; i++) {
        PntDvdTr::inquire(i);
        printf("%lld\n", f[i]);
    }
}