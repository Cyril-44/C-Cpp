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
using namespace std;using pii_t=pair<int,int>;using pll_t=pair<int64_t,int64_t>;using veci_t=vector<int>;using vecl_t=vector<int64_t>;Inline int Popcnt(int x){return __builtin_popcount((unsigned)x);}Inline int Popcnt(unsigned x){return __builtin_popcount(x);}Inline int Popcnt(int64_t x){return __builtin_popcountll((uint64_t)x);}Inline int Popcnt(uint64_t x){return __builtin_popcountll(x);}Inline int Log2(int x){return 31-__builtin_clz((unsigned)x|1);}Inline int Log2(unsigned x){return 31-__builtin_clz(x|1);}Inline int Log2(int64_t x){return 63-__builtin_clzll((uint64_t)x|1);}Inline int Log2(uint64_t x){return 63-__builtin_clzll(x|1);}

#define MULTI_TEST_CASES

constexpr int N = 8005;
int fa[N];
int find(int u) { return fa[u] ^ u ? (fa[u] = find(fa[u])) : u; }
inline bool unite(int u, int v) { return u = find(u), v = find(v), (u == v) ? false : (fa[u] = v); }

bitset<N> con[N], trans[N];
int deg[N];

int n;
vector<pair<int,int>> ans;
bool vis[N];
bool flg;
void dfs(int u) {
    if (!flg || vis[u]) return;
    vis[u] = true;
    trans[u][u] = true;
    while (true) {
        int mxp = 0;
        for (int v = 1; v <= n; v++)
            if (con[u][v] && !trans[u][v] && deg[v] > deg[mxp])
                mxp = v;
        for (int i = 1; i <= n; i++)
            if (!con[u][i] && con[mxp][i]) { /* cerr << u << ' ' << mxp << ' ' << i << "---\n";  */flg = false; return; }
        if (!mxp) return;
        ans.emplace_back(u, mxp);
        if (!unite(u, mxp)) { /* cerr << u << ' ' << mxp << "+++\n";  */flg = false; return; }
        dfs(mxp);
        trans[u] |= trans[mxp];
        if (!flg) break;
    }
}
inline void solveSingleTestCase() {
    cin >> n;
    For (i, 1, n) trans[i].reset();
    For (i, 1, n) {
        string s;
        cin >> s;
        For (j, 1, n) con[i][j] = s[j-1] & 1;
        deg[i] = con[i].count();
    }
    ans.clear();
    memset(vis, 0, n+1);
    flg = true;
    iota(fa+1, fa+1+n, 1);
    For (i, 1, n) {
        if (!con[i][i]) NO;
        For (j, 1, n)
            if (i != j && con[i][j] && con[j][i]) NO;
    }
    while (true) {
        int mxp = 0;
        For (i, 1, n) if (!vis[i] && deg[i] > deg[mxp]) mxp = i;
        if (!mxp) break;
        dfs(mxp);
        if (!flg) break;
    }
    flg &= (ans.size() == n-1);
    if (!flg) NO;
    cout << "Yes\n";
    for (const auto &[u, v] : ans) cout << u << ' ' << v << '\n';
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