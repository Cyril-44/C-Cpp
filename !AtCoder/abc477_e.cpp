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

constexpr int N = 200005;
int a[N*2], b[N*2];
int64_t c[N*2], s1[N*2], s2[N*2], d[N*2];
struct Max{
    constexpr static int K = 20;
    int64_t st[N*2][K];
    inline void init(int64_t *a, int n) {
        For(i, 1, n) st[i][0] = a[i];
        For(k, 1, 19)
            For(i, 1, n - (1<<k) + 1)
                st[i][k] = min(st[i][k-1], st[i + (1<<k-1)][k-1]);
    }
    inline int64_t operator()(int l, int r) const {
        int k = 31 - __builtin_clz(r - l + 1);
        return min(st[l][k], st[r - (1<<k) + 1][k]);
    }
} st;
inline void solveSingleTestCase() {
    int n, q;
    cin >> n >> q;
    For(i, 1, n) cin >> a[i], a[i+n] = a[i];
    For(i, 1, n) cin >> b[i], b[i+n] = b[i];
    For(i, 1, n*2) s1[i+1] = s1[i] + a[i];
    For(i, 1, n*2) c[i] = s1[i] + b[i];
    st.init(c, n*2);
    For(i, 1, n) d[i] = st(i, i+n) - s1[i];
    roF(i, n*2, 1) s2[i-1] = s2[i] + a[i-1];
    For(i, 1, n*2) c[i] = s2[i] + b[i];
    st.init(c, n*2);
    For(i, 1, n) d[i] = min(d[i], st(i, i+n) - s2[i+n]);
    Repv(q, s, t) {
        cin >> s >> t;
        if (t == n+1) cout << d[s];
        else {
            int64_t ans = min({s1[t] - s1[s], s1[s+n] - s1[t], d[s] + d[t]});
            // cerr << s1[t] - s1[s] << ' ' << s1[s+n] - s1[t] << ' ' << d[s] + d[t] << '\n';
            cout << ans;
        }
        cout << '\n';
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