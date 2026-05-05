#include <cstdio>
#include <algorithm>
#include <limits>
#include <numeric>
#include <vector>
#include <cstring>
#include <functional>
constexpr int N = 100005;
#define L l.val
#define R r.val
#define Lid l.id
#define Rid r.id
struct PNode { struct Node {int val, id;} l, r; int id; } a[N], a2[N], a3[N], a4[N];
struct UnionFind {
    int fa[N], sz[N];
    UnionFind(int n) { std::iota(fa+1, fa+1+n, 1), std::fill(sz+1, sz+1+n, 1); }
    inline int find(int u) { return (fa[u] ^ u) ? (fa[u] = find(fa[u])) : u; }
    inline int size(int u) { return sz[find(u)]; }
    inline bool unite(int u, int v) {
        u = find(u), v = find(v);
        if (u ^ v) return fa[u] = v, sz[v] += sz[u];
        else return false;
    }
};
template<typename Comp, int INF>
struct Data {
    struct DataNode {int v=INF, id=0;} a[2];
    inline DataNode& operator[](bool x) { return a[x]; }
    inline DataNode operator[](bool x) const { return a[x]; }
    inline void pull(int m, int i) {
        if (Comp{}(m, a[0].v)) {
            if (i == a[0].id) a[0].v = m;
            else a[1] = a[0], a[0].v = m, a[0].id = i;
        } else if (Comp{}(m, a[1].v) && i != a[0].id)
            a[1].v = m, a[1].id = i;
    }
    template<typename Node> inline void pull(const Node &x) { pull(x.v, x.id); }
    template<typename Dat> inline Data& operator*=(const Dat &o) {
        if (o[0].id) {
            pull(o[0]);
            if (o[1].id) 
                pull(o[1]);
        }
        return *this;
    }
};
template<typename Dat, bool Inv>
struct BIT {
    std::vector<Dat> tr;
    BIT(int n) : tr(n+1) {}
    inline void upd(int u, int m, int id) {
        if (Inv) u = (int)tr.size() - u;
        // printf("Update %d by (%d,%d)\n", u, m, id);
        for (; u < (int)tr.size(); u += u & -u) tr[u].pull(m, id);
    }
    Dat sum(int u) {
        if (Inv) u = (int)tr.size() - u;
        Dat res;
        // printf("Inquire %d\n", u);
        for (; u; u -= u & -u) res *= tr[u];
        return res;
    }
};
#ifdef DEBUG
# define dbgf(...) fprintf(stderr, __VA_ARGS__)
# define DBG(s) \
    dbgf(s"\n"); \
    for (int i = 1; i <= n; i++) { \
        dbgf("%d-->%d ", i, mn[i].to); \
        if (mn[i].mn == std::numeric_limits<int>::max()) dbgf("INF\n"); \
        else dbgf("%d\n", mn[i].mn); \
    }
#else
# define dbgf(...) ((void)0)
# define DBG(s) ((void)0)
#endif
struct MnNode { int mn, fr, to; } mn[N];
template<typename VL>
inline void update(MnNode& var, const VL& val, int id) {
    if (val[0].id && val[0].id != id) {
        dbgf("Update v=%d (#%d) to #%d\n", val[0].v, val[0].id, id);
        if (var.mn > val[0].v) var.mn = val[0].v, var.to = val[0].id;
    } else if (val[1].id && val[1].id != id) {
        dbgf("Update v=%d (#%d) to #%d\n", val[1].v, val[1].id, id);
        if (var.mn > val[1].v) var.mn = val[1].v, var.to = val[1].id;
    }
}
int main() {
    int n, m;
    scanf("%d", &n);
    {
        static int arr[N<<1];
        for (int i = 1; i <= n; i++) {
            scanf("%d%d", &a[i].L, &a[i].R);
            arr[i*2-1] = a[i].L, arr[i*2] = a[i].R;
        }
        std::sort(arr+1, arr+1 + 2*n);
        m = std::unique(arr+1, arr+1 + 2*n) - arr-1;
        for (int i = 1; i <= n; i++) {
            a[i].id = i;
            a[i].Lid = std::lower_bound(arr+1, arr+1 + m, a[i].L) - arr;
            a[i].Rid = std::lower_bound(arr+1, arr+1 + m, a[i].R) - arr;
        }
    }
    memcpy(a2+1, a+1, sizeof(*a) * n);
    memcpy(a3+1, a+1, sizeof(*a) * n);
    memcpy(a4+1, a+1, sizeof(*a) * n);
    std::sort(a+1, a+1+n, [](const PNode &x, const PNode &y) { return x.R < y.R || x.R == y.R && x.L > y.L; });
    std::sort(a2+1, a2+1+n, [](const PNode &x, const PNode &y) { return x.L < y.L || x.L == y.L && x.R > y.R; });
    std::sort(a3+1, a3+1+n, [](const PNode &x, const PNode &y) { return x.L > y.L || x.L == y.L && x.R < y.R; });
    std::sort(a4+1, a4+1+n, [](const PNode &x, const PNode &y) { return x.R > y.R || x.R == y.R && x.L < y.L; });
    dbgf("===== a1 =====");
    for (int i = 1; i <= n; i++) dbgf("%d %d\n", a[i].L, a[i].R);
    dbgf("===== a2 =====");
    for (int i = 1; i <= n; i++) dbgf("%d %d\n", a2[i].L, a2[i].R);
    UnionFind dsu(n);
    int64_t ans = 0;
    while (dsu.size(1) != n) {
        for (int i = 1; i <= n; i++)
            mn[i] = {std::numeric_limits<int>::max(), i, 0};
        {  // L <= l < r <= R
            BIT<Data<std::less<int>, std::numeric_limits<int>::max()>, true> f(m);
            for (int i = 1; i <= n; i++) {
                int id = dsu.find(a[i].id);
                auto res = f.sum(a[i].Lid);
                update(mn[id], res, id);
                f.upd(a[i].Lid, a[i].R - a[i].L, id);
            }
            DBG("L <= l <= r <= R");
        }{ // l <= L < r <= R
            BIT<Data<std::less<int>, std::numeric_limits<int>::max()>, true> f(m);
            for (int i = 1; i <= n; i++) {
                int id = dsu.find(a2[i].id);
                auto res = f.sum(a2[i].Lid + 1);
                if (res[0].id && (res[0].v -= a2[i].L) >= a2[i].R - a2[i].L) res[0].id = 0;
                if (res[1].id && (res[1].v -= a2[i].L) >= a2[i].R - a2[i].L) res[1].id = 0;
                update(mn[id], res, id);
                f.upd(a2[i].Rid, a2[i].R, id);
            }
            DBG("l <= L < r <= R");
        }{ // L <= l < R <= r
            BIT<Data<std::greater<int>, std::numeric_limits<int>::min()>, false> f(m);
            for (int i = 1; i <= n; i++) {
                int id = dsu.find(a3[i].id);
                auto res = f.sum(a3[i].Rid - 1);
                if (res[0].id && (res[0].v = a3[i].R - res[0].v) >= a3[i].R - a3[i].L) res[0].id = 0;
                if (res[1].id && (res[1].v = a3[i].R - res[1].v) >= a3[i].R - a3[i].L) res[1].id = 0;
                update(mn[id], res, id);
                f.upd(a3[i].Lid, a3[i].L, id);
            }
            DBG("L <= l <= R <= r");
        }{ // l <= L < R <= r
            BIT<Data<std::greater<int>, std::numeric_limits<int>::min()>, false> f(m);
            for (int i = 1; i <= n; i++) {
                int id = dsu.find(a4[i].id);
                auto res = f.sum(a4[i].Lid);
                if (res[0].id) res[0].v = a4[i].R - a4[i].L;
                if (res[1].id) res[1].v = a4[i].R - a4[i].L;
                update(mn[id], res, id);
                f.upd(a4[i].Lid, a4[i].L, id);
            }
            DBG("L <= l < r <= R");
        }
        // std::sort(mn+1, mn+1 + n, [](const MnNode &x, const MnNode &y){ return x.mn < y.mn; });
        for (int i = 1; i <= n; i++) {
            if (mn[i].to && dsu.unite(i, mn[i].to)) {
                dbgf("Unite %d with %d, value=%d\n", i, mn[i].to, mn[i].mn);
                ans += mn[i].mn;
            }
        }
    }
    printf("%ld\n", ans);

    return 0;
}