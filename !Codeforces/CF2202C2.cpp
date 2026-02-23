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

constexpr int N = 300005;
int a[N];
inline void solveSingleTestCase() {
    int n;
    cin >> n;
    For (i, 1, n) cin >> a[i];
    long long ans = 0;
    for (int i = 1; i <= n; i++) {
        int j = i;
        map<int, int> hit;
        hit[a[j]] = j;
        ans += 1ll * i * (n - i + 1);
        fprintf(stderr, "At Point %d, add %lld\n", i, 1ll * i * (n - i + 1));
        if (j < n && a[j+1] == a[i] + 1) {
            int mn = a[i] + 1;
            for (; j < n && (a[j+1] == a[j] + 1 || a[j+1] >= mn && a[j+1] <= a[j]); ++j) { 
                if (a[j+1] <= a[j]) {
                    ans += 1ll * (j+1 - hit[a[j+1]] + 1) * (n - (j+1) + 1);
                } else {
                    hit[a[j+1]] = j+1;
                    ans += (n - (j+1) + 1);
                }
                // fprintf(stderr, "At Point %d, add %lld\n", j+1, (j+1 - hit[a[j+1]] + 1) * (n - (j+1) + 1));
            }
            i = j;
        }
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
/*
10
2 3 7 8 9 8 9 10 8 9 
Exp: 91, Found: 104
*/