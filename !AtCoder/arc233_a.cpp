#include <bits/stdc++.h>
#include <random>
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
#define EXIT(s...) return (cout << s), void()
using namespace std;using pii_t=pair<int,int>;using pll_t=pair<int64_t,int64_t>;using veci_t=vector<int>;using vecl_t=vector<int64_t>;Inline int Popcnt(int x){return __builtin_popcount((unsigned)x);}Inline int Popcnt(unsigned x){return __builtin_popcount(x);}Inline int Popcnt(int64_t x){return __builtin_popcountll((uint64_t)x);}Inline int Popcnt(uint64_t x){return __builtin_popcountll(x);}Inline int Log2(int x){return 31-__builtin_clz((unsigned)x|1);}Inline int Log2(unsigned x){return 31-__builtin_clz(x|1);}Inline int Log2(int64_t x){return 63-__builtin_clzll((uint64_t)x|1);}Inline int Log2(uint64_t x){return 63-__builtin_clzll(x|1);}
namespace Solution{
#define MULTI_TEST_CASES

mt19937 rng(random_device{}());
uniform_int_distribution<int> bgen(0,1);

constexpr int N = 200005;

bitset<N> f;
inline void globalInit() {
}

inline bool inquire(int x) {
    cout << "? " << x << endl;
    string res; cin >> res;
    return res == "Yes";
}
bool flg[N];
inline void solveSingleTestCase() {
    int n;
    cin >> n;
    memset(flg, 0, n+1);
    if (n % 2 == 0 && !inquire(n / 2)) EXIT("! 2" << endl);
    else flg[n/2] = true;
    int mid = (n-1)/2, cnt = 0;
    For(i, 1, mid) cnt += flg[i] = inquire(i);
    For(i, n/2+1, n-1)
        if ((cnt <= mid/2) == flg[n-i]) {
            flg[i] = inquire(i);
            if (flg[i] == flg[n-i]) EXIT("! 2" << endl);
        }
        else flg[i] = !flg[n-i];
    f.reset();
    f.set(0);
    For(i, 1, mid) if (flg[i]) f |= f << i;
    if (f.test(n)) EXIT("! 3" << endl);
    For(i, mid+1, n) if (flg[i] && f.test(n-i)) EXIT("! 3" << endl);
    EXIT("! -1" << endl);
}
}
int main() {
    cin.tie(nullptr) -> sync_with_stdio(false);
    Solution::globalInit();
    int testCases = 1;
#ifdef MULTI_TEST_CASES
    cin >> testCases;
#endif
    while (testCases--) Solution::solveSingleTestCase();
    return 0;
}