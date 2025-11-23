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
int x[N], y[N];
int _x[N], _y[N];
struct Node {
    int x, y, id;
    inline bool operator< (const Node &rhs) const {
        return id < rhs.id;
    }
} a[N];
inline void solveSingleTestCase() {
    int n, m;
    cin >> n >> m;
    For (i, 1, n) cin >> x[i];
    For (i, 1, m) cin >> y[i];
    memcpy(_x, x, sizeof(int) * (n+1));
    memcpy(_y, y, sizeof(int) * (m+1));
    sort(_x+1, _x+1 + n);
    sort(_y+1, _y+1 + m);
    For (i, 1, n) if (_x[i] < i * m) NO;
    For (i, 1, m) if (_y[i] < i * n) NO;
    iota(_x, _x+n*m, 1);
    set<int> st(_x, _x+n*m);
    cout << "Yes\n";
    int tot = 0;
    For (i, 1, n)
        For (j, 1, m) {
            int limit = min(x[i], y[j]);
            a[++tot] = {i, j, min(x[i], y[j])};
        }
    sort(a+1, a+1 + tot);
    vector<vector<int>> ans(n+1, vector<int>(m+1));
    For (i, 1, tot) {
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