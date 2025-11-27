#include <bits/stdc++.h>
using namespace std;
using pii_t = pair<int, int>;
using pll_t = pair<int64_t, int64_t>;
using veci_t = vector<int>;
using vecl_t = vector<int64_t>;
#ifndef ONLINE_JUDGE
# pragma GCC optimize(2, 3, "inline", "unroll-loops")
# pragma GCC target("tune=native")
#endif
#define Inline __attribute__((always_inline)) inline
#define For(i, s, t) for (int i = (s); i <= (t); ++i)
#define Forv(i, s, t, ...) for (int i = (s), __VA_ARGS__; i <= (t); ++i)
#define roF(i, t, s) for (int i = (t); i >= (s); --i)
#define roFv(i, t, s, ...) for (int i = (t), __VA_ARGS__; i >= (s); --i)
#define Rep(c) for (int tempFor_count = c; tempFor_count; --tempFor_count)
#define YES return cout << "Yes\n", void()
#define NO return cout << "No\n", void()
#define YESNO(j) cout << ((j) ? "Yes\n" : "No\n")
Inline int Popcnt(int x) { return __builtin_popcount((unsigned)x); }
Inline int Popcnt(unsigned x) { return __builtin_popcount(x); }
Inline int Popcnt(int64_t x) { return __builtin_popcountll((uint64_t)x); }
Inline int Popcnt(uint64_t x) { return __builtin_popcountll(x); }
Inline int Log2(int x) { return 31 - __builtin_clz((unsigned)x | 1); }
Inline int Log2(unsigned x) { return 31 - __builtin_clz(x | 1); }
Inline int Log2(int64_t x) { return 63 - __builtin_clzll((uint64_t)x | 1); }
Inline int Log2(uint64_t x) { return 63 - __builtin_clzll(x | 1); }

// #define MULTI_TEST_CASES

constexpr int N = 500005;
int64_t a[N];
inline void solveSingleTestCase() {
    int n, c;
    int64_t m;
    cin >> n >> m >> c;
    For (i, 1, n) cin >> a[i];
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