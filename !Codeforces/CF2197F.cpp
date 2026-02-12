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
/*
注意到两种括号互不影响，先分开来看看
遇到非法的括号，把他们拎出来，比如 )))(((，这里是有办法使用 len/2 的次数变成合法的。
但是如果两组的非法括号刚刚好**都是奇数**（不可能是偶数，n - 配对()*2 - 配对[]*2 一定是偶数）
此时，可以 ]](( 同样 len/2 次数。
但是注意 如果遇到这种 ]( 那必须要两次，也就是 len/2+1
所以对于这种特判一下就好了
取不合法的最后面的 ] 与最前面的 (，如果 pos] < pos( 就要ans++
同理 )[，)(，](
两个同时需要 ans++ 的时候，ans才可以++，因为注意到当前是奇数，所以两两配对，如果一个可以另外一种就不用考虑了，两种配对都不行才要 ans++
*/
inline void solveSingleTestCase() {
    int n;
    cin >> n >> s;
    int x = 0, y = 0, fx = 0, fy = 0;
    int lastr = 0, frontxl = n+1, frontyl = n+1;
    For (i, 1, n) switch (s[i-1]) {
           case '[': ++x; if (x == 1) frontxl = i;
    break; case ']': --x; if (x == -1) ++fx, x = 0, lastr = i; if (!x) frontxl = n+1;
    break; case '(': ++y; if (y == 1) frontyl = i;
    break; case ')': --y; if (y == -1) ++fy, y = 0, lastr = i; if (!y) frontyl = n+1;
    }
    int ans = (x + y + fx + fy) / 2;
    if ((x + y) & 1 && lastr < std::min(frontxl, frontyl)) ++ans;
    cout << ans << '\n';
}
int main() {
    int testCases = 1;
    cin.tie(nullptr) -> sync_with_stdio(false);
#ifdef MULTI_TEST_CASES
    cin >> testCases;
#endif
    while (testCases--) solveSingleTestCase();
    return 0;
}