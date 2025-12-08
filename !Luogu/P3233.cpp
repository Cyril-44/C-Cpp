#include <bits/stdc++.h>
#include <random>
#define LUOGU
#if defined(ONLINE_JUDGE) && !defined(LUOGU)
#pragma GCC optimize(2, 3, "inline", "unroll-loops", "fast-math", "inline-small-functions", "no-stack-protector",      \
                     "delete-null-pointer-checks")
#pragma GCC target("tune=native")
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
using namespace std;using pii_t=pair<int,int>;using pll_t=pair<int64_t,int64_t>;using veci_t=vector<int>;using vecl_t=vector<int64_t>;Inline int Popcnt(int x){return __builtin_popcount((unsigned)x);}Inline int Popcnt(unsigned x){return __builtin_popcount(x);}Inline int Popcnt(int64_t x){return __builtin_popcountll((uint64_t)x);}Inline int Popcnt(uint64_t x){return __builtin_popcountll(x);}Inline int Log2(int x){return 31-__builtin_clz((unsigned)x|1);}Inline int Log2(unsigned x){return 31-__builtin_clz(x|1);}Inline int Log2(int64_t x){return 63-__builtin_clzll((uint64_t)x|1);}Inline int Log2(uint64_t x){return 63-__builtin_clzll(x|1);}

// #define MULTI_TEST_CASES

constexpr int N = 300005, K = 19;
int a[N];
int fa[N][K + 1], dfn[N], dep[N];
vector<int> g[N], ng[N];
bool key[N];
namespace Init {
int ts = 0;
void dfs(int u) {
    dfn[u] = ++ts;
    For(k, 1, K) fa[u][k] = fa[fa[u][k - 1]][k - 1];
    for (int v : g[u]) {
        if (v == fa[u][0]) continue;
        fa[v][0] = u;
        dep[v] = dep[u] + 1;
        dfs(v);
    }
}
} // namespace Init

inline int jmp(int u, int l) {
    for (int dg; l; l ^= 1 << dg) {
        dg = __builtin_ctz(l);
        u = fa[u][dg];
    }
    return u;
}
inline int lca(int u, int v) {
    if (dep[u] < dep[v]) std::swap(u, v);
    u = jmp(u, dep[u] - dep[v]);
    if (u == v) return u;
    For (k, K, 0)
        if (fa[u][k] != fa[v][k])
            u = fa[u][k], v = fa[v][k];
    return fa[u][0];
}
inline int len(int u, int v) {
    return abs(dep[u] - dep[v]);
}

namespace Solve {
pair<int,int> f[N];
void dfsdown(int u) {
    for (int v : ng[u]) {
        
    }
}
void dfsup(int u) {

}
}

namespace VT {
int sta[N];
inline void build(std::vector<int> &pts) {
    std::sort(pts.begin(), pts.end(), [](int x, int y) { return dfn[x] < dfn[y]; });
    if (pts.front() == 1) pts.erase(pts.begin());
    int top;
    sta[top = 1] = 1;
    for (int v : pts) {
        int com = lca(v, sta[top]);
        if (sta[top] != com) {
            for (; dfn[sta[top - 1]] >= dfn[com]; --top) ng[sta[top - 1]].push_back(sta[top]);
            if (sta[top-1] != com) ng[com].push_back(sta[top]), sta[top] = com;
        }
        sta[++top] = v;
    }
}
inline void clean(int u = 1) {
    for (int v : ng[u]) clean(v);
    key[u] = false;
    ng[u].clear();
    Solve::f[u] = pair<int,int>();
}
} // namespace VT
inline void solveSingleTestCase() {
    int n;
    cin >> n;
    for (int i = 1, u, v; i < n; i++) {
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    Init::dfs(1);
    int q;
    cin >> q;
    for (int mi; q--; ) {
        cin >> mi;
        vector<int> pts;
        pts.reserve(mi);
        Repv (mi, x) {
            cin >> x;
            pts.push_back(x);
            VT::build(pts);
            
            VT::clean();
        }
    }
}
int main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    int testCases = 1;
#ifdef MULTI_TEST_CASES
    cin >> testCases;
#endif
    while (testCases--) solveSingleTestCase();
    return 0;
}