#include <cstdio>
#include <algorithm>
#include <numeric>
#include <vector>
#include <map>
constexpr int N = 100005;
using pii_t = std::pair<int,int>;
int n;
#define L first
#define R second
pii_t a[N];
struct UnionFind {
    int fa[N];
    UnionFind(int n) { std::iota(fa+1, fa+1+n, 1); }
    inline int find(int u) { return (fa[u] ^ u) ? (fa[u] = find(fa[u])) : u; }
    inline bool unite(int u, int v) {
        u = find(u), v = find(v);
        if (u ^ v) return fa[u] = v;
        else return false;
    }
};
inline void bfsolve() {
    UnionFind dsu(n);
    int cnt = 1;
    struct Edge { int w, u, v; };
    std::vector<Edge> edgs;
    edgs.reserve(n*(n-1)/2);
    for (int i = 1; i < n; i++)
        for (int j = i+1; j <= n; j++) {
            int l = std::max(a[i].L, a[j].L), r = std::min(a[i].R, a[j].R);
            if (l < r) edgs.push_back({r - l, i, j});
        }
    std::sort(edgs.begin(), edgs.end(), [](const Edge&x, const Edge&y){ return x.w < y.w; });
    int64_t ans = 0;
    for (const auto &e : edgs) {
        if (dsu.unite(e.u, e.v)) {
            ++cnt;
            ans += e.w;
            if (cnt == n) break;
        }
    }
    printf("%ld\n", ans);
}
struct Sub2 {
    int f[N];
    inline void operator()() {
        std::sort(a+1, a+1+n, [](const pii_t&x, const pii_t&y){ return x.R-x.L < y.R-y.L; });
        std::map<int,int> mp;
        for (int i = 1; i <= n; i++) f[i] = a[i].R - a[i].L;
        for (int i = 1; i <= n; i++) {
            const auto itl = mp.lower_bound(a[i].L), itr = mp.lower_bound(a[i].R);
            for (auto it = itl; it != itr; it = mp.erase(it))
                f[i] = std::min(f[i], f[it->second]);
            mp.emplace(a[i].L, i);
        }
        auto ans = std::accumulate(f+1, f+n, 0ll);
        printf("%lld\n", ans);
    }
};
int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        scanf("%d%d", &a[i].L, &a[i].R);
    if (n <= 1000) bfsolve();
    else Sub2{}();
    return 0;
}