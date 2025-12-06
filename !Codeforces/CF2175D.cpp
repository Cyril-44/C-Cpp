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

#define umax(x, y) ((x) = max((x), (y)))

constexpr int N = 100005, M = 365;
int a[N];
inline void solveSingleTestCase() {
    int n, k;
    cin >> n >> k;
    For (i, 1, n) cin >> a[i];
    int m = 0;
    vector<int> cnt(k+1, 0);
    For (i, 1, n) {
        if (a[m] < a[i]) a[++m] = a[i];
        ++cnt[m];
    }
    vector<vector<int>> f[2], g; // 【最大值】【总和】
    f[0].assign(k+1, vector<int>(k+1, -1));
    f[1].assign(k+1, vector<int>(k+1, -1));
    f[0][0][0] = 0;
    For (i, 1, m) {
        // clog << cnt[i] << "====\n";
        auto& pre = f[i&1^1], &now = f[i&1];
        For (j, 0, k)
            For (s, 0, k)
                if (~pre[j][s])
                    now[j][s] = pre[j][s] + j * cnt[i];
        g = pre;
        For (j, 1, k)
            For (s, 0, k)
                if (~g[j-1][s])
                    umax(g[j][s], g[j-1][s]);
        For (j, 0, a[i])
            For (s, j, k)
                if (~g[j][s - j])
                    umax(now[j][s], g[j][s - j] + j * cnt[i]);
        // For (j, 0, k) For (s, 0, k) clog << now[j][s] << (s==k?'\n':' '); clog << "--------\n";
    }
    int ans = 0;
    For (j, 0, k)
        For (s, 0, k) {
            // clog << f[m&1][j][s] << (s==k?'\n':' ');
            umax(ans, f[m&1][j][s]);
        }
    // clog << "========\n";
    cout << ans << '\n';
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