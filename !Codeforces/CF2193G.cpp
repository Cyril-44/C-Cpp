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

constexpr int N = 200005;
vector<int> g[N];
int fa[N], dep[N], fmx[N];
bool vis[N];
int n;
map<pair<int,int>,int> result;
inline int query(int u, int v) {
    if (u > v) swap(u, v);
    if (result.count({u, v})) return result[{u, v}];
    cout << "? " << u << ' ' << v << endl;
    int res; cin >> res; if(res==-1)exit(0);
    return result[{u, v}] = res;
}
void dfs(int u, int pre) {
    fmx[u] = u;
    for (int v : g[u]) {
        if (v == pre) continue;
        fa[v] = u, dep[v] = dep[u] + 1;
        dfs(v, u);
        if (dep[v] > dep[fmx[u]]) fmx[u] = v;
    }
}
inline void solveChain(int x, int y) {
    memset(vis, 0, sizeof(int) * (n+1));
    for (int fx = x; fx; fx = fa[fx]) vis[fx] = true;
    int anc = y;
    while (!vis[anc]) anc = fa[anc];
    vector<int> chain;
    for (int fx = x; fx != anc; fx = fa[fx]) chain.push_back(fx);
    chain.push_back(anc);
    int pos = chain.size();
    for (int fy = y; fy != anc; fy = fa[fy]) chain.push_back(fy);
    reverse(chain.begin() + pos, chain.end());
    auto lt = chain.begin(), rt = chain.end();
    while (lt < rt) {
        auto mid = lt + (rt - lt - 1) / 2;
        if (query(*lt, *mid)) rt = mid;
        else lt = mid;
    }
    cout << "! " << *lt << endl;
}
bool solve(int u, int pre) {
    vector<int> vec; vec.reserve(g[u].size());
    for (int v : g[u]) {
        if (v == pre) continue;
        vec.push_back(fmx[v]);
    }
    if (pre) {
        for (auto it = vec.begin(); it != vec.end(); ++it)
            if (*it == fmx[u]) { vec.erase(it); break; }
    }
    for (int i = 1; i < (int)vec.size(); i += 2)
        if (query(vec[i-1], vec[i])) return solveChain(vec[i-1], vec[i]), true;
    if ((vec.size() & 1) && query(u, vec.back()))
        return solveChain(u, vec.back()), true;
    for (int v : g[u]) {
        if (v == pre) continue;
        if (solve(v, u)) return true;
    }
    return false;
}
inline void solveSingleTestCase() {
    cin >> n;
    For (i, 1, n) g[i].clear();
    Repv (n-1, u, v) {
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    result.clear();
    if (n == 2) {
        if (query(1,1)) cout << "! 1" << endl;
        else cout << "! 2" << endl;
    } else if (n == 3) {
        if (!query(1,2)) cout << "! 3" << endl;
        else if (query(1,1)) cout << "! 1" << endl;
        else cout << "! 2" << endl;
    } else {
        dfs(1, 0);
        solve(1, 0);
    }
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