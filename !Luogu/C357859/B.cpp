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
#define EXIT(s...) return (cout << s), void();
using namespace std;using pii_t=pair<int,int>;using pll_t=pair<int64_t,int64_t>;using veci_t=vector<int>;using vecl_t=vector<int64_t>;Inline int Popcnt(int x){return __builtin_popcount((unsigned)x);}Inline int Popcnt(unsigned x){return __builtin_popcount(x);}Inline int Popcnt(int64_t x){return __builtin_popcountll((uint64_t)x);}Inline int Popcnt(uint64_t x){return __builtin_popcountll(x);}Inline int Log2(int x){return 31-__builtin_clz((unsigned)x|1);}Inline int Log2(unsigned x){return 31-__builtin_clz(x|1);}Inline int Log2(int64_t x){return 63-__builtin_clzll((uint64_t)x|1);}Inline int Log2(uint64_t x){return 63-__builtin_clzll(x|1);}

#define MULTI_TEST_CASES

constexpr int N = 300005;
int a[N];
struct Op { int c, x; } op[N];
int n, m;
inline int mex(int x, int y) {
    if (x >= 1 && y >= 1) return 0;
    if (!x&&y==1 || !y&&x==1) return 2;
    return 1;
}
inline bool simu() {
    static int b[N];
    memcpy(b, a, sizeof(int) * (n+1));
    int trans[4]{0,1,2,3};
    int gcdnum = 0;
    For(i, 1, m) {
        if (op[i].c == 1) For(x, 0, 3) trans[x] = mex(trans[x], op[i].x);
        else {
            gcdnum = __gcd(gcdnum, op[i].x);
            if (i == m || op[i+1].c != 2) {
                if (trans[3] != 3) For(j, 1, n) b[j] = trans[min(b[j], 3)];
                For(j, 1, n) b[j] = __gcd(b[j], gcdnum);
                trans[0]=0, trans[1]=1, trans[2]=2, trans[3]=3;
            }
        }
    }
    if (trans[3] != 3) For(j, 1, n) b[j] = trans[min(b[j], 3)];
    For(j, 2, n) if (b[j] != b[1]) return false;
    return true;
}

inline void solveSingleTestCase() {
    cin >> n >> m;
    For(i, 1, n) cin >> a[i];
    For(i, 1, m) cin >> op[i].c >> op[i].x;
    int last = m + 1;
    roF(i, m, 1) if (op[i].c != 2) { last = i; break; }
    if (last <= m) {
        For(i, 1, last-1) if (op[i].c != 1) YES;
        if (!op[last].c) {
            op[last].c = 1;
            if (simu()) YES;
            op[last].c = 2;
        }
    }
    YESNO(simu());
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