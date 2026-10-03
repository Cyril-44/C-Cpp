#include <bits/stdc++.h>
// #define LUOGU
#if defined(ONLINE_JUDGE) && !defined(LUOGU)
# pragma GCC optimize(2, 3, "inline", "unroll-loops", "fast-math", "inline-small-functions", "no-stack-protector", "delete-null-pointer-checks")
# pragma GCC target("tune=native")
#endif
#define Inline __attribute__((always_inline)) inline
#define For(i, s, t) for (int i = (s); i <= (t); ++i)
#define Forv(i, s, t, ...) for (int i = (s), __VA_ARGS__; i <= (t); ++i)
#define roF(i, t, s) for (int i = (t); i >= (s); --i)
#define roFv(i, t, s, ...) for (int i = (t), __VA_ARGS__; i >= (s); --i)
#define Rep(c) for (int tempFor_count = c; tempFor_count; --tempFor_count)
#define Repv(c, ...) for (int tempFor_count = c, __VA_ARGS__; tempFor_count; --tempFor_count)
#define YES return cout << "Yes\n", void()
#define NO return cout << "No\n", void()
#define YESNO(j) cout << ((j) ? "Yes\n" : "No\n")
#define EXIT(s...) return (cout << s), void();
using namespace std;using pii_t=pair<int,int>;using pll_t=pair<int64_t,int64_t>;using veci_t=vector<int>;using vecl_t=vector<int64_t>;Inline int Popcnt(int x){return __builtin_popcount((unsigned)x);}Inline int Popcnt(unsigned x){return __builtin_popcount(x);}Inline int Popcnt(int64_t x){return __builtin_popcountll((uint64_t)x);}Inline int Popcnt(uint64_t x){return __builtin_popcountll(x);}Inline int Log2(int x){return 31-__builtin_clz((unsigned)x|1);}Inline int Log2(unsigned x){return 31-__builtin_clz(x|1);}Inline int Log2(int64_t x){return 63-__builtin_clzll((uint64_t)x|1);}Inline int Log2(uint64_t x){return 63-__builtin_clzll(x|1);}
namespace Solution{
// #define MULTI_TEST_CASES

#ifdef CLANGD
constexpr int N = 25;
#else
constexpr int N = 200005;
#endif
set<pii_t> edg0, edg1;
vector<int> g0[N], g1[N], g2[N];
bool vis[N];
struct UFS {
    int fa[N];
    int find(int u) { return u == fa[u] ? u : (fa[u] = find(fa[u])); }
    bool unite(int u, int v) {
        u = find(u), v = find(v);
        if (u == v) return false;
        fa[u] = v; return true;
    }
} ufs;
int deg[N], a[N];
inline void solveSingleTestCase() {
    int n, q;
    cin >> n >> q;
    Repv(q, t, u, v) {
        cin >> t >> u >> v;
        if (t) {
            ++deg[v], g1[u].push_back(v), ufs.unite(u, v);
            edg1.emplace(u, v);
        } else {
            edg0.emplace(u, v);
        }
    }
    for (auto [u, v] : edg0) {
        if (edg0.count({v, u})) {
            if (u < v) ++deg[v], g2[u].push_back(v);
        }
        else {
            ++deg[v], g0[u].push_back(v);
        }
    }
    queue<int> que;
    fill(a+1, a+1+n, 1);
    For(i, 1, n) if (!deg[i]) que.push(i);
    while (!que.empty()) {
        int u = que.front(); que.pop();
        for (int v : g0[u]) {
            if (--deg[v] == 0) que.push(v);
            a[v] = max(a[v], a[u]);
        }
        for (int v : g1[u]) {
            if (--deg[v] == 0) que.push(v);
            a[v] = max(a[v], a[u] + 1);
        }
        for (int v : g2[u]) {
            if (--deg[v] == 0) que.push(v);
            a[u] = a[v] = max(a[u], a[v]);
        }
    }
    for (auto [u, v] : edg0) if (a[u] > a[v]) NO;
    for (auto [u, v] : edg1) if (a[u] >= a[v]) NO;
    cout << "Yes\n";
    For(i, 1, n) cout << a[i] << ' ';
    cout << '\n';
}
}
int main() {
    cin.tie(nullptr) -> sync_with_stdio(false);
    int testCases = 1;
#ifdef MULTI_TEST_CASES
    cin >> testCases;
#endif
    while (testCases--) Solution::solveSingleTestCase();
    return 0;
}