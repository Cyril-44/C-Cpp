#include <bits/stdc++.h>
#define LUOGU
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

constexpr int N = 100005;
char sa[N], sb[N], sc[N];
int ans[N];
inline void solveSingleTestCase() {
    int n;
    cin >> n >> sa >> sb >> sc;
    vector<int> premx({1}), sufmx({n});
    set<int> mids;
    For (i, 2, n) if (sa[i-1] ^ '0') premx.push_back(i);
    roF (i, n-1, 1) if (sb[i-1] ^ '0') sufmx.push_back(i);
    For (i, 1, n) if (sc[i-1] ^ '0') mids.insert(i);
    reverse(sufmx.begin(), sufmx.end());
    if (premx.back() > sufmx.front()) return void(cout << "-1\n");
    for (int i : premx) if (mids.count(i)) return void(cout << "-1\n");
    for (int i : sufmx) if (mids.count(i)) return void(cout << "-1\n");
    For (i, 1, sufmx.front()-1) {
        int j = i;
        while (j < n && sc[j] == '1') ++j;
        ans[i] = j;
        For (p, i+1, j) ans[p] = p-1;
        i = j;
    }
    int j = sufmx.front(), v1 = n, v2 = sufmx.front();
    for (int i : sufmx) {
        ans[i] = v1--;
        For (p, j, i-1) ans[p] = v2++;
        j = i+1;
    }
    For (i, 1, n) cout << ans[i] << ' ';
    cout << '\n';
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