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
int a[2][N];
int l[N], r[N], g[N], f[N], st[N][20];
inline void init(int n) {
    For (i, 1, n)
        st[i][0] = f[i];
    For (k, 1, 19)
        For (i, 1, n - (1<<k) + 1)
            st[i][k] = min(st[i][k-1], st[i + (1<<k-1)][k-1]);
}
inline int getmn(int l, int r) {
    int k = Log2(r - l + 1);
    return min(st[l][k], st[r - (1 << k) + 1][k]);
}
inline void solveSingleTestCase() {
    int n;
    cin >> n;
    For (j, 0, 1) For (i, 1, n) cin >> a[j][i];
    l[0] = 2 * n;
    For (i, 1, n) l[i] = min(l[i-1], a[0][i]);
    r[n+1] = 2 * n;
    roF (i, n, 1) r[i] = min(r[i+1], a[1][i]);
    g[n+1] = 0;
    roF (i, n, 1) g[i] = max(g[i+1], a[1][i]);
    f[0] = 0;
    For (i, 1, n) f[i] = max(f[i-1], a[0][i]);
    For (i, 1, n) f[i] = max(f[i], g[i]);
    init(n);
    int64_t ans = 0;
    For (xl, 1, n<<1) {
        int p1 = std::upper_bound(l+1, l+1 + n, xl, greater<int>()) - l - 1;
        int p0 = std::lower_bound(r+1, r+1 + n, xl) - r;
        if (p0 > p1) break;
        ans += n * 2 - getmn(p0, p1) + 1;
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