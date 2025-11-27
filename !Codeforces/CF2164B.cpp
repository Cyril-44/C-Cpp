#include <bits/stdc++.h>
// #define LUOGU
#if defined(ONLINE_JUDGE) && !defined(LUOGU)
# pragma GCC optimize(2, 3, "inline", "unroll-loops")
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

constexpr int N = 100005;
int a[N], even[N], odd[N];
inline void solveSingleTestCase() {
    int n;
    cin >> n;
    For (i, 1, n) cin >> a[i];
    int et(0), ot(0);
    For (i, 1, n) {
        if (a[i] & 1) odd[++ot] = a[i];
        else even[++et] = a[i];
    }
    if (et >= 2) {
        cout << even[1] << ' ' << even[2] << '\n';
        return;
    }
    else if (et == 1) {
        For (i, 1, ot) {
            int a = max(even[1], odd[i]), b = min(even[1], odd[i]);
            if (a % b & 1 ^ 1) {
                cout << b << ' ' << a << '\n';
                return;
            }
        }
    }
    For (i, 1, ot)
        For (j, max(1, i - 100), i-1)
            if (odd[i] % odd[j] & 1 ^ 1) {
                cout << odd[j] << ' ' << odd[i] << '\n';
                return;
            }
    cout << "-1\n";
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