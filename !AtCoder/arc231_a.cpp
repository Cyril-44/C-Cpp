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
#define MULTI_TEST_CASES

constexpr int N = 505;
int x[N*N], y[N*N], z[N*N];
int64_t g[N][N];
inline void globalInit() {
    
}

inline void solveSingleTestCase() {
    int n;
    cin >> n;
    int64_t zsum = 0, ans = 0;
    For(i, 1, n) cin >> x[i] >> y[i] >> z[i], zsum += z[i];
    memset(g, 0x3f, sizeof g);
    For(x, 0, 499) g[x][0] = x*x;
    For(p, 1, n) {
        int64_t cur = 1ll<<60;
        For(j, 0, 499) cur = min(cur, g[x[p]][j] + (y[p]-j)*(y[p]-j));
        ans = min(ans, cur -= z[p]);
        For(i, 0, 499) g[i][y[p]] = min(g[i][y[p]], cur + (x[p]-i)*(x[p]-i));
    }
    printf("%lld\n", ans + zsum);
}
}
int main() {
    cin.tie(nullptr) -> sync_with_stdio(false);
    Solution::globalInit();
    int testCases = 1;
#ifdef MULTI_TEST_CASES
    cin >> testCases;
#endif
    while (testCases--) Solution::solveSingleTestCase();
    return 0;
}