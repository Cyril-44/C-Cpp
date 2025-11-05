#include <bits/stdc++.h>
using namespace std;
using pii_t = pair<int, int>;
using pll_t = pair<int64_t, int64_t>;
using veci_t = vector<int>;
using vecl_t = vector<int64_t>;
#define LUOGU
#if defined(ONLINE_JUDGE) && !defined(LUOGU)
# pragma GCC optimize(2, 3, "inline", "unroll-loops")
# pragma GCC target("tune=native")
#endif
#define Inline __attribute__((always_inline)) inline
#define For(i, s, t) for (int i = (s); i <= (t); ++i)
#define Forv(i, s, t, ...) for (int i = (s), __VA_ARGS__; i <= (t); ++i)
#define roF(i, t, s) for (int i = (t); i >= (s); --i)
#define roFv(i, t, s, ...) for (int i = (t), __VA_ARGS__; i >= (s); --i)
#define Rep(c) for (int tempFor_count = c; tempFor_count; --tempFor_count)
#define YES return cout << "Yes\n", void()
#define NO return cout << "No\n", void()
#define YESNO(j) cout << ((j) ? "Yes\n" : "No\n")
Inline int Popcnt(int x) { return __builtin_popcount((unsigned)x); }
Inline int Popcnt(unsigned x) { return __builtin_popcount(x); }
Inline int Popcnt(int64_t x) { return __builtin_popcountll((uint64_t)x); }
Inline int Popcnt(uint64_t x) { return __builtin_popcountll(x); }
Inline int Log2(int x) { return 31 - __builtin_clz((unsigned)x | 1); }
Inline int Log2(unsigned x) { return 31 - __builtin_clz(x | 1); }
Inline int Log2(int64_t x) { return 63 - __builtin_clzll((uint64_t)x | 1); }
Inline int Log2(uint64_t x) { return 63 - __builtin_clzll(x | 1); }

// #define MULTI_TEST_CASES

constexpr int N = 10015, M = 1000005;

struct Edge {
    int u, v, w;
    inline bool operator< (const Edge &rhs) const {
        return w < rhs.w;
    }
} edgs[M];
int c[N];
namespace DSU {
    int fa[N];
    inline void init(int n) { For (i, 1, n) fa[i] = i; }
    inline int getfa(int u) {
        int v = u, tp;
        while (u ^ fa[u]) u = fa[u];
        while (v ^ u) tp = fa[v], fa[v] = u, v = tp;
        return u;
    }
    inline bool merge(int u, int v) {
        u = getfa(u), v = getfa(v);
        return (u ^ v) ? (fa[v] = u) : false; 
    }
}
inline void solveSingleTestCase() {
    int n, m, k;
    cin >> n >> m >> k;
    For (i, 1, m) cin >> edgs[i].u >> edgs[i].v >> edgs[i].w;
    sort(edgs+1, edgs+1+m);
    DSU::init(n);
    int cnt = 1;
    For (i, 1, m) { // 先 Kruskal
        if (DSU::merge(edgs[i].u, edgs[i].v))
            edgs[cnt++] = edgs[i];
        if (cnt == n) break;
    }
    For (i, 1, k) {
        cin >> c[i];
        Forv (j, 1, n, wi) {
            cin >> wi;
            edgs[cnt++] = Edge {i + n, j, wi};
        }
    }
    sort(edgs + 1, edgs + cnt);
    long long ans = 1ll << 62;
    For (st, 0, (1<<k) - 1) {
        DSU::init(n + k);
        int const exp = Popcnt(st) + n;
        int tot = 1;
        long long res = 0;
        For (i, 1, cnt-1) {
            if ((edgs[i].u <= n || (st >> (edgs[i].u - n - 1) & 1)) && DSU::merge(edgs[i].u, edgs[i].v))
                ++tot, res += edgs[i].w;
            if (tot == exp) break;
        }
        if (tot != exp) continue;
        For (i, 1, k) if (st >> (i-1) & 1) res += c[i];
        ans = min(ans, res);
    }
    printf("%lld\n", ans);
}
int main() {
    cin.tie(nullptr) -> sync_with_stdio(false);
    int testCases = 1;
#ifdef MULTI_TEST_CASES
    cin >> testCases;
#endif
    while (testCases--) solveSingleTestCase();
    return 0;
}