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
int a[N];
int64_t s[N];
inline void solveSingleTestCase() {
    int n, l, r;
    cin >> n >> l >> r;
    For (i, 1, n) cin >> a[i];
    sort(a+1, a+1 + n);
    For (i, 1, n) s[i] = s[i-1] + a[i];
    int L = upper_bound(a+1, a+1 + n, l) - a-1, 
        R = lower_bound(a+1, a+1 + n, r) - a;
    if (L > (n+1)/2) {
        int a = n-L+1, b = L;
        cout << -s[a-1] + s[n] - s[b] + l * (b-a+1ll) - (s[b] - s[a-1]) << '\n';
    } else if (R <= n/2) {
        int a = R, b = n-R+1;
        cout << -s[a-1] + s[n] - s[b] + (s[b] - s[a-1]) - r * (b-a+1ll) << '\n';
    } else {
        int64_t ans = -s[n/2] + s[n] - s[(n+1)/2];
        if (n & 1) {
            if (a[n/2+1] <= l) ans += l - a[n/2+1];
            else if (a[n/2+1] >= r) ans += a[n/2+1] - r;
        }
        cout << ans << '\n';
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