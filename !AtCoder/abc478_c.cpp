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
// #define MULTI_TEST_CASES

constexpr int N = 200005;
int a[N], mx[N], mn[N];
bool f[N], b[N];
inline void solveSingleTestCase() {
    int n, k;
    cin >> n >> k;
    For(i, 1, n) cin >> a[i];
    // For(i, 1, n) cin >> p[i].first, p[i].second = i;
    // sort(p+1, p+1+n);
    // For(i, 1, n) a[p[i].second] = i;
    mn[n+1] = a[n+1] = n+1;
    f[0] = b[n+1] = true;
    For(i, 1, n) f[i] = f[i-1] && (a[i-1] <= a[i]);
    roF(i, n, 1) b[i] = b[i+1] && (a[i] <= a[i+1]);
    roF(i, n, 1) mn[i] = min(mn[i+1], a[i]);
    For(i, 1, n) mx[i] = max(mx[i-1], a[i]);
    For(i, 1, n-k+1) {
        if (f[i-1] && b[i+k] && mx[i-1] <= mn[i] && mx[i+k-1] <= mn[i+k]) YES;
    }
    NO;
}
}
int main() {
    cin.tie(nullptr) -> sync_with_stdio(false);
    int testCases = 1;
#ifdef MULTI_TEST_CASES
    cin >> testCases;
#endif
    while (testCases--) Solution::solveSingleTestCase();
    return 0;
}