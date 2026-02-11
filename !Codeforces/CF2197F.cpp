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
char s[N];
int x[N], y[N];
inline void solveSingleTestCase() {
    int n;
    cin >> n >> s;
    For (i, 1, n) switch (s[i-1]) {
        case '[': ++x[i]; break;
        case ']': --x[i]; break;
        case '(': ++y[i]; break;
        case ')': --y[i]; break;
    }
    For (i, 1, n) x[i] += x[i-1], y[i] += y[i-1];
    int xp = min_element(x+1, x+1 + n) - x,
        yp = min_element(y+1, y+1 + n) - y;
    if (x[xp] > 0 && y[yp] < 0) {
        cout << (x[n] + (y[n] - y[yp] * 2)) / 2 << '\n';
    } else if (x[xp] < 0 && y[yp] > 0) {
        cout << ((x[n] - x[xp] * 2) + y[n]) / 2 << '\n';
    } else if (x[xp] > 0 && y[yp] > 0) {
        cout << (x[n] + y[n]) / 2 << '\n';
    } else {
        cout << ((x[n] - x[xp] * 2) + (y[n] - y[yp] * 2)) / 2 << '\n';
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