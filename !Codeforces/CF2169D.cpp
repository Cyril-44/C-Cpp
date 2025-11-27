#include <bits/stdc++.h>
#include <bits/stdint-intn.h>
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

constexpr int N = 0;

#ifndef ONLINE_JUDGE
inline int bruteForce() {
    int x, y;
    cin >> x >> y;
    vector<int> arr(1000);
    iota(arr.begin(), arr.end(), 1);
    set<int> st(arr.begin(), arr.end());
    For (i, 1, x) {
        auto it = st.begin();
        cerr << "Round " << i << ": ";
        for (;;) {
            Rep(y-1) {
                if (it == st.end()) break;
                ++it;
            }
            if (it == st.end()) break;
            cerr << *it % y << ' ';
            it = st.erase(it);
        }
        cerr << '\n';
    }
    for (int i : st) cerr << i << ' ';
    cerr << endl;
    return 114514;
}
// auto ___ = bruteForce();
#endif

int64_t x, y, k;
inline int64_t getcnt (__int128 cnt) {
    // Rep (x) cerr << (cnt -= cnt / y) << '\n';
    int64_t xx = x;
    while (xx > 0 && cnt >= y) {
        int64_t tmp = cnt / y;
        int64_t blksz = (cnt - tmp * y) / tmp + 1;
        blksz = min(blksz, xx);
        cnt -= tmp * blksz;
        xx -= blksz;
        if (cnt < k) return cnt;
    }
    return cnt;
};
inline void solveSingleTestCase() {
    cin >> x >> y >> k;
    if (getcnt(1e12) < k) return (cout << "-1\n"), void();
    int64_t l = 0, r = 1e12, mid;
    while (l <= r) {
        mid = l + r >> 1;
        if (getcnt(mid) < k) l = mid + 1;
        else r = mid - 1;
    }
    cout << l << '\n';
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