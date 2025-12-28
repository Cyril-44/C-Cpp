#include <bits/stdc++.h>
#include <numeric>
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
pii_t a[N];
inline void solveSingleTestCase() {
    int n, m;
    cin >> n >> m;
    For (i, 1, n) {
        cin >> a[i].first;
        a[i].second = i;
    }
    sort(a+1, a+1 + n);
    if (m > n/2) return (cout << "-1\n"), void();
    if (m) {
        cout << n-m << '\n';
        For (i, 1, m-1)
            cout << a[i+m-1].second << ' ' << a[i].second << '\n';
        For (i, m<<1, n)
            cout << a[i].second << ' ' << a[i-1].second << '\n';
    } else {
        int64_t sm1 = 0;
        For (i, 1, n-1) sm1 += a[i].first;
        if (sm1 < a[n].first) return (cout << "-1\n"), void();
        int mn = n; int64_t sum = 0;
        while (mn > 1 && sum + a[mn-1].first <= a[n].first) sum += a[--mn].first;
        cout << n-1 << '\n';
        For (i, 2, mn) cout << a[i-1].second << ' ' << a[i].second << '\n';
        For (i, mn, n-1) cout << a[i].second << ' ' << a[n].second << '\n';
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