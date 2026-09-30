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

// #define MULTI_TEST_CASES

#ifdef CLANGD
constexpr int N = 2;
#else
constexpr int N = 200005;
#endif
constexpr int B = 640;
int val[N], ans[N], n;
vector<int> g[N];
struct Query { int s, t, ex, a, b, lid, rid, id; } q[N];
struct Sum {
    int a[N], b[N];
    inline void add(int p, int x) { a[p] += x, b[p / B] += x; }
    inline int sum(int p) { int sum = 0, blk = p / B; For(i, 0, blk-1) sum += b[i]; For(i, blk*B, p) sum += a[i]; return sum; }
    inline int sum(int l, int r) { return sum(r) - sum(l-1); }
} fs;
struct DfbLCA {
    constexpr static int K = 17;
    int dfn[N], top, st[N][K+1];
    inline int argu(int u, int v) const {
        return dfn[u] < dfn[v] ? u : v;
    }
    void dfs(int u, int fa) {
        dfn[u] = ++top, st[top][0] = fa;
        for (int v : g[u]) if (v != fa) dfs(v, u);
    }
    inline void init() {
        top = 0, dfs(1, 0);
        For(k, 1, K)
            For(i, 1, n - (1<<k)+1)
                st[i][k] = argu(st[i][k-1], st[i+(1<<k-1)][k-1]);
    }
    inline int operator()(int u, int v) const {
        if (u == v) return u;
        u = dfn[u], v = dfn[v];
        if (u > v) std::swap(u, v);
        int k = __lg(v - u++);
        return argu(st[u][k], st[v-(1<<k)+1][k]);
    }
} lca;
int seq[N*2], mp[N][2], top;
void dfs(int u, int fa) {
    seq[++top] = u, mp[u][0] = top;
    for (int v : g[u]) if (v != fa) dfs(v, u);
    seq[++top] = u, mp[u][1] = top;
}
int cnt[N]; bool vis[N];
inline void change(int x) {
    fs.add(cnt[val[x]], -1);
    cnt[val[x]] += vis[x] ? -1 : 1;
    fs.add(cnt[val[x]], 1);
    vis[x] = !vis[x];
}
inline void solveSingleTestCase() {
    int m;
    cin >> n >> m;
    For(i, 1, n) cin >> val[i];
    Repv(n-1, u, v) {
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    dfs(1, 0);
    lca.init();
    For(i, 1, m) {
        cin >> q[i].s >> q[i].t >> q[i].a >> q[i].b;
        if (mp[q[i].s][0] > mp[q[i].t][0]) std::swap(q[i].s, q[i].t);
        q[i].ex = lca(q[i].s, q[i].t);
        if (q[i].s == q[i].ex)
            q[i].s = mp[q[i].s][0], q[i].t = mp[q[i].t][0], q[i].ex = 0;
        else
            q[i].s = mp[q[i].s][1], q[i].t = mp[q[i].t][0];
        q[i].lid = q[i].s / B, q[i].rid = q[i].t / B, q[i].id = i;
    }
    sort(q+1, q+1+m, [](const Query&x, const Query&y) {
        return x.lid < y.lid || x.lid == y.lid && (x.lid & 1 ? x.rid > y.rid : x.rid < y.rid);
    });
    int l=1, r=0;
    For(i, 1, m) {
        while (r < q[i].t) change(seq[++r]);
        while (l > q[i].s) change(seq[--l]);
        while (r > q[i].t) change(seq[r--]);
        while (l < q[i].s) change(seq[l++]);
        if (q[i].ex) change(q[i].ex);
        ans[q[i].id] = fs.sum(q[i].a, q[i].b);
        if (q[i].ex) change(q[i].ex);
    }
    For(i, 1, m) cout << ans[i] << '\n';
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