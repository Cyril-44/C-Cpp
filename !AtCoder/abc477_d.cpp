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
#define EXIT(s...) return (cout << s), void();
using namespace std;using pii_t=pair<int,int>;using pll_t=pair<int64_t,int64_t>;using veci_t=vector<int>;using vecl_t=vector<int64_t>;Inline int Popcnt(int x){return __builtin_popcount((unsigned)x);}Inline int Popcnt(unsigned x){return __builtin_popcount(x);}Inline int Popcnt(int64_t x){return __builtin_popcountll((uint64_t)x);}Inline int Popcnt(uint64_t x){return __builtin_popcountll(x);}Inline int Log2(int x){return 31-__builtin_clz((unsigned)x|1);}Inline int Log2(unsigned x){return 31-__builtin_clz(x|1);}Inline int Log2(int64_t x){return 63-__builtin_clzll((uint64_t)x|1);}Inline int Log2(uint64_t x){return 63-__builtin_clzll(x|1);}

// #define MULTI_TEST_CASES

constexpr int N = 300005;
struct OP {int op, x; char c;} a[N];
char col[N];
inline void solveSingleTestCase() {
    int n, q;
    cin >> n >> q;
    std::set<int> on, off;
    For(i, 1, n) on.insert(i);
    For(i, 1, q) {
        cin >> a[i].op;
        if (a[i].op == 1) {
            cin >> a[i].x;
            if (off.count(a[i].x)) off.erase(a[i].x), on.insert(a[i].x);
            else if (on.count(a[i].x)) on.erase(a[i].x), off.insert(a[i].x);
        }
        else cin >> a[i].c;
    }
    roF(i, q, 1) {
        if (a[i].op == 1) {
            if (off.count(a[i].x)) off.erase(a[i].x), on.insert(a[i].x);
            else if (on.count(a[i].x)) on.erase(a[i].x), off.insert(a[i].x);
        } else {
            for (int x : on) col[x] = a[i].c;
            on.clear();
        }
    }
    For(i, 1, n) if (!col[i]) col[i] = 'a';
    cout << (col+1);
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