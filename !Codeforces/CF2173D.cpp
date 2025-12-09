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
#define umax(x, y) ((x) = max((x), (y)))
using namespace std;using pii_t=pair<int,int>;using pll_t=pair<int64_t,int64_t>;using veci_t=vector<int>;using vecl_t=vector<int64_t>;Inline int Popcnt(int x){return __builtin_popcount((unsigned)x);}Inline int Popcnt(unsigned x){return __builtin_popcount(x);}Inline int Popcnt(int64_t x){return __builtin_popcountll((uint64_t)x);}Inline int Popcnt(uint64_t x){return __builtin_popcountll(x);}Inline int Log2(int x){return 31-__builtin_clz((unsigned)x|1);}Inline int Log2(unsigned x){return 31-__builtin_clz(x|1);}Inline int Log2(int64_t x){return 63-__builtin_clzll((uint64_t)x|1);}Inline int Log2(uint64_t x){return 63-__builtin_clzll(x|1);}

#define MULTI_TEST_CASES

constexpr int N = 34;
int f[N][N][2];
inline void solveSingleTestCase() {
    int n, k;
    cin >> n >> k;
    array<int, 32> g;
    g.fill(0);
    memset(f, 0, sizeof f);
    roFv (i, 30, 0, len = 0) {
        if (n >> i & 1) ++len;
        else len = 0;
        g[i] = len;
    }
    For (i, 0, 31) { // f 要取后缀 max
        For (j, 0, 30) {
            umax(f[i + g[i] + 1][j+1][1], f[i][j][1] + g[i] + 1);
            umax(f[i + g[i] + 1][j+1][1], f[i][j][0] + g[i]);
        }
        For (ti, i+1, 31)
            For (j, 0, 31)
                umax(f[ti][j][0], max(f[ti-1][j][1], f[ti-1][j][0]));
    }
    array<int, 32> fmx;
    fmx.fill(0);
    For (i, 0, 31)
        For (j, 0, 31)
            umax(fmx[j], max(f[i][j][0], f[i][j][1]));
    int ans = 0;
    For (j, 0, 31) {
        if (j > k) break;
        umax(ans, fmx[j] + k - j - 1);
    }
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