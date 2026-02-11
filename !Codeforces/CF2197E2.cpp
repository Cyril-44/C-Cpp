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

constexpr int N = 35;
map<int, vector<int>> res;
inline vector<int> query(int ord) {
    if (res.count(ord)) return res[ord];
    auto &ans = res[ord];
    cout << "? " << ord << endl;
    int q;
    cin >> q;
    ans.resize(q);
    for (int &i : ans) cin >> i;
    return ans;
}
int cap[N];
vector<pair<int,int>> edgs;
void dfs(int u, int idx, int& ord) {
    if (~cap[u]) return ord += cap[u], void();
    int start = ord;
    for (auto vec = query(ord); (int)vec.size() > idx && vec[idx] == u; vec = query(ord)) {
        if ((int)vec.size() > idx + 1) {
            edgs.emplace_back(vec[idx], vec[idx + 1]);
            dfs(vec[idx + 1], idx + 1, ord);
        }
        else ++ord;
    }
    cap[u] = ord - start;
}
inline void solveSingleTestCase() {
    int n;
    cin >> n;
    memset(cap, -1, sizeof cap);
    res.clear();
    edgs.clear();
    res[1] = {1};
    // if (query(n + 1).empty()) {
    //     cout << "! 0" << endl;
    //     return ;
    // }
    int ord = 1;
    for (int i = 1; i <= n; i++) dfs(i, 0, ord);
    cout << "! " << edgs.size() << endl;
    for (const auto &[u, v] : edgs)
        cout << u << ' ' << v << endl;
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