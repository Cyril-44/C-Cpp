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

// #define MULTI_TEST_CASES

constexpr int N = 1000005;
int f[N], v[N], c[N];
long long pre1[N], pre2[N];
int st[N][20][2], mp[N][2];
inline void initst(int n) {
    For (i, 1, n) {
        st[i][0][0] = mp[c[i]][0];
        st[i][0][1] = mp[c[i]][1];
    }
    For (k, 1, 20)
        For (i, 1, n - (1<<k) + 1) {
            st[i][k][0] = min(st[i][k-1][0], st[i + (1<<k-1)][k-1][0]);
            st[i][k][1] = max(st[i][k-1][1], st[i + (1<<k-1)][k-1][1]);
        }
}
inline pair<int,int> query(int l, int r) {
    int k = Log2(r - l + 1);
    r -= (1<<k) - 1;
    return make_pair(min(st[l][k][0], st[r][k][0]), max(st[l][k][1], st[r][k][1]));
}
std::pair<int,int> rg[N];
int sum[N];
inline void solveSingleTestCase() {
    int n;
    cin >> n;
    For (i, 1, n) cin >> c[i];
    For (i, 1, n) cin >> v[i];
    For (i, 1, n) cin >> f[i];
    For (i, 1, n) {
        if (!mp[c[i]][0]) mp[c[i]][0] = i;
        mp[c[i]][1] = i;
    }
    initst(n);
    uint64_t __debug_all_count = 0;
    int64_t ans = numeric_limits<int64_t>::max();
    int tot = 0;
    For (i, 1, n) {
        if (!mp[i][0]) continue;
        auto [l, r] = query(mp[i][0], mp[i][1]);
        if (l == mp[i][0])
            rg[++tot] = {l, r};
    }
    For (i, 1, tot) ++sum[rg[i].first];
    For (i, 1, n) sum[i] += sum[i-1];
    For (i, 1, tot) {
        if (sum[rg[i].second] - sum[rg[i].first] == 0) {
            int64_t now = 0;
            __debug_all_count += rg[i].second - rg[i].first + 1;
            For (j, rg[i].first, rg[i].second)
                now += v[j] * f[j - rg[i].first + 1];
#ifdef DEBUG
            clog << "For " << i << ": [" << rg[i].first << ", " << rg[i].second << "] = " << now << endl;
#endif
            ans = min(ans, now);
        }
    }
    cout << ans << '\n';
    clog << "CALC " << __debug_all_count << " times." << endl;
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