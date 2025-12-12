#include <bits/stdc++.h>
#define LUOGU
#if defined(ONLINE_JUDGE) && !defined(LUOGU)
#pragma GCC optimize(2, 3, "inline", "unroll-loops", "fast-math", "inline-small-functions", "no-stack-protector",      \
                     "delete-null-pointer-checks")
#pragma GCC target("tune=native")
#endif
#define Inline __attribute__((always_inline)) inline
#define For(i, s, t) assert((s) <= (t)); for (int i = (s); i <= (t); ++i)
#define Forv(i, s, t, ...) for (int i = (s), __VA_ARGS__; i <= (t); ++i)
#define roF(i, t, s) assert((t) >= (s)); for (int i = (t); i >= (s); --i)
#define roFv(i, t, s, ...) for (int i = (t), __VA_ARGS__; i >= (s); --i)
#define Rep(c) for (int tempFor_count = c; tempFor_count; --tempFor_count)
#define Repv(c, ...) for (int tempFor_count = c, __VA_ARGS__; tempFor_count; --tempFor_count)
#define YES return cout << "Yes\n", void()
#define NO return cout << "No\n", void()
#define YESNO(j) cout << ((j) ? "Yes\n" : "No\n")
using namespace std;using pii_t=pair<int,int>;using pll_t=pair<int64_t,int64_t>;using veci_t=vector<int>;using vecl_t=vector<int64_t>;Inline int Popcnt(int x){return __builtin_popcount((unsigned)x);}Inline int Popcnt(unsigned x){return __builtin_popcount(x);}Inline int Popcnt(int64_t x){return __builtin_popcountll((uint64_t)x);}Inline int Popcnt(uint64_t x){return __builtin_popcountll(x);}Inline int Log2(int x){return 31-__builtin_clz((unsigned)x|1);}Inline int Log2(unsigned x){return 31-__builtin_clz(x|1);}Inline int Log2(int64_t x){return 63-__builtin_clzll((uint64_t)x|1);}Inline int Log2(uint64_t x){return 63-__builtin_clzll(x|1);}

// #define MULTI_TEST_CASES
#ifdef DEBUG
#define Debug(...) fprintf(stderr, __VA_ARGS__)
#else
#define Debug(...) ((void)0)
#endif
constexpr int N = 300005, K = 19;
int a[N];
int fa[N][K + 1], dfn[N], dep[N], sz[N];
vector<int> g[N], ng[N];
bool key[N];
namespace Init {
int ts = 0;
void dfs(int u) {
    sz[u] = 1;
    dfn[u] = ++ts;
    For(k, 1, K) fa[u][k] = fa[fa[u][k - 1]][k - 1];
    for (int v : g[u]) {
        if (v == fa[u][0]) continue;
        fa[v][0] = u;
        dep[v] = dep[u] + 1;
        dfs(v);
        sz[u] += sz[v];
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
    if (dep[u] < dep[v]) swap(u, v);
    u = jmp(u, dep[u] - dep[v]);
    if (u == v) return u;
    roF (k, K, 0)
        if (fa[u][k] != fa[v][k])
            u = fa[u][k], v = fa[v][k];
    return fa[u][0];
}
inline int len(int u, int v) {
    return abs(dep[u] - dep[v]);
}

int ans[N];
namespace Solve {
pair<int,int> f[N]; // (离 i 最近的关键点 到 i 的距离，离 i 最近的关键点编号)
void calc(const int o, const int u) { // 计算虚数上从 o 到 u 对 f[o].second 和 f[u].second 的贡献
    int dvd = u; // 这里的分割点是指 u 能最远覆盖到的点
    assert(o != u);
    roF (k, K, 0) {
        int to = fa[dvd][k];
        int ulen = len(u, to) + f[u].first;
        int olen = len(to, o) + f[o].first;
        if (dep[to] > dep[o] && (ulen < olen || ulen == olen && f[u].second < f[o].second))
            dvd = to;
    }
    Debug("(#%d)_%d_(sz=%d,dep=%d) <== %d_(sz=%d,dep=%d) <== (#%d)_%d_(sz=%d,dep=%d): %d -= %d, %d += %d\n", 
        f[u].second, u, sz[u], dep[u], dvd, sz[dvd], dep[dvd], f[o].second, o, sz[o], dep[o], ans[f[o].second], sz[dvd], ans[f[u].second], sz[dvd] - sz[u]);
    // assert(ans[f[o].second] >= sz[dvd]);
    ans[f[o].second] -= sz[dvd]; // 去掉不必要的部分
    ans[f[u].second] += sz[dvd] - sz[u];
}
void dfsup(int u = 1) { // 用儿子更新父亲
    f[u] = {(int)1e9, 0};
    for (int v : ng[u]) {
        dfsup(v);
        auto tmp = f[v];
        tmp.first += len(u, v);
        f[u] = min(f[u], tmp);
    }
    if (key[u]) f[u] = {0, u};
}
void dfsdown(int u = 1) { // 用父亲更新儿子
    ans[f[u].second] += sz[u];
    for (int v : ng[u]) {
        auto tmp = f[u];
        tmp.first += len(u, v);
        f[v] = min(f[v], tmp);
        dfsdown(v);
        calc(u, v);
    }
}
}

namespace VT {
int sta[N];
inline void build(vector<int> pts) {
    sort(pts.begin(), pts.end(), [](int x, int y) { return dfn[x] < dfn[y]; });
    if (pts.front() == 1) pts.erase(pts.begin());
    int top;
    sta[top = 1] = 1;
    for (int v : pts) {
        int com = lca(v, sta[top]);
        if (sta[top] != com) {
            for (; dfn[sta[top - 1]] >= dfn[com]; --top) ng[sta[top - 1]].push_back(sta[top]);
            if (sta[top] != com) ng[com].push_back(sta[top]), sta[top] = com;
        }
        sta[++top] = v;
    }
    for (int i = 1; i < top; i++)
        ng[sta[i]].push_back(sta[i+1]);
}
inline void clean(int u = 1) {
    for (int v : ng[u]) clean(v);
    key[u] = false;
    ng[u].clear();
    ans[u] = 0;
}
inline void debug(int u = 1) {
    for (int v : ng[u]) {
        Debug("%d %d\n", u, v);
        debug(v);
    }
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
            key[x] = true;
            assert(ans[x] == 0);
        }
        VT::build(pts); VT::debug(); Debug("===================================\n");
        Solve::dfsup();
        Solve::dfsdown();
        for (int i : pts) printf("%d ", ans[i]); putchar('\n');
        VT::clean();
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