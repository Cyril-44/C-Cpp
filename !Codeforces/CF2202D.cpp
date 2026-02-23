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
#define pb push_back
inline void solveSingleTestCase() {
        int n;
        int k;
        cin >> n >> k;
        int lo = n;
        int hi = 2LL * n - 1;
        if (k < lo || k > hi) {
            cout << "NO\n";
            return;
        }
        int m = k - n;
        vector<int> a;
        a.reserve(2 * n);
        if (m == 0) {
            for (int i = 1; i <= n; i++) {
                a.pb(i);
                a.pb(i);
            }
        } else {
            int p = (int)m + 1;
            if (p == 2) {
                a = {1, 2, 1, 2};
            } else {
                a.pb(1);
                a.pb(2);
                a.pb(1);
                for (int i = 2; i <= p - 1; i++) {
                    a.pb(i + 1);
                    a.pb(i);
                }
                a.pb(p);
            }

            for (int j = p + 1; j <= n; j++) {
                a.pb(j);
                a.pb(j);
            }
        }
        if ((int)a.size() != 2 * n) {
            cout << "NO\n";
        }
        cout << "YES\n";
        for (int i = 0; i < 2 * n; i++) {
            cout << a[i] << (i + 1 == 2 * n ? '\n' : ' ');
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