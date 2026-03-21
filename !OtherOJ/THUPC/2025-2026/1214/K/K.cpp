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

// #define MULTI_TEST_CASES

constexpr int N = 1000005;
int a[N], b[N];
inline void solveSingleTestCase() {
    int n, m;
    long long L, as, bs;
    cin >> n >> m >> L >> as >> bs;
    For (i, 1, n) cin >> a[i];
    For (i, 1, m) cin >> b[i];
    sort(a+1, a+1+n), sort(b+1, b+1+m);
    For (i, 2, n) if (a[i] - a[i-1] > as) { puts("-1"); return; }
    For (i, 2, m) if (b[i] - b[i-1] > bs) { puts("-1"); return; }
    vector<int> all;
    all.push_back(0);
    all.reserve(n+m);
    int last = 1;
    For (i, 2, n) {
        if (a[i+1] > a[last] + as) {
            last = i;
            all.push_back(a[i]);
        }
    }
    last = 1;
    For (i, 2, m) {
        if (b[i+1] > b[last] + bs) {
            last = i;
            all.push_back(b[i]);
        }
    }
    sort(all.begin(), all.end());
    auto end = unique(all.begin(), all.end());
    cout << end - all.begin() + 1 << endl;
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