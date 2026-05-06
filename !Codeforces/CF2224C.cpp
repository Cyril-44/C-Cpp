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
char a[N], b[N];
int sa[N], sb[N];
int oa[N], ob[N];
inline void solveSingleTestCase() {
    int n;
    cin >> n >> a >> b;
    For (i, 1, n) {
        sa[i] = sa[i-1], sb[i] = sb[i-1];
        if (a[i-1] == '(') ++sa[i]; else --sa[i];
        if (b[i-1] == '(') ++sb[i]; else --sb[i];
    }
    // oa[n+1] = n+1, ob[n+1] = n+1;
    // roF (i, n, 1) oa[i] = min(oa[i+1], sa[i]);
    // roF (i, n, 1) ob[i] = min(ob[i+1], sb[i]);
    // For (i, 1, n) oa[i] = max(oa[i-1], oa[i]);
    // For (i, 1, n) ob[i] = max(ob[i-1], ob[i]);
    if (sa[n] + sb[n] != 0 || sa[n] % 2) NO;
    int suma = 0, sumb = 0;
    For (i, 1, n) {
        if (a[i-1] == '(' && b[i-1] == '(') ++suma, ++sumb;
        else if (a[i-1] == ')' && b[i-1] == ')') --suma, --sumb;
        else {
            if (suma > sumb) ++sumb, --suma;
            else ++suma, --sumb;
        }
        if (suma < 0 || sumb < 0) NO;
    }
    YES;
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