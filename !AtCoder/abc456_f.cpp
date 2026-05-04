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
int a[N], k;
int64_t f0[N][2], f1[N][2], g0[N][2], g1[N][2];
int64_t ans;
void solve(int l, int r) {
    if (r - l + 1 < k) return;
    if (l == r) {
        ans = std::min(ans, 1l*a[l]);
        return;
    }
    int mid = l + r >> 1;
    solve(l, mid), solve(mid + 1, r);
    f0[mid][0] = a[mid], f0[mid][1] = 1ll << 60;
    f1[mid][0] = a[mid], f1[mid][1] = 0;
    roF (i, mid - 1, l) {
        f0[i][0] = min(f0[i+1][0], f0[i+1][1]) + a[i], f0[i][1] = f0[i+1][0];
        f1[i][0] = min(f1[i+1][0], f1[i+1][1]) + a[i], f1[i][1] = f1[i+1][0];
    }
    g0[mid+1][0] = a[mid+1], g0[mid+1][1] = 1ll << 60;
    g1[mid+1][0] = a[mid+1], g1[mid+1][1] = 0;
    For (i, mid + 2, r) {
        g0[i][0] = min(g0[i-1][0], g0[i-1][1]) + a[i], g0[i][1] = g0[i-1][0];
        g1[i][0] = min(g1[i-1][0], g1[i-1][1]) + a[i], g1[i][1] = g1[i-1][0];
    }
    // For (i, l, mid) printf("%d: %ld %ld, %ld %ld\n", i, f0[i][0], f0[i][1], f1[i][0], f1[i][1]);
    // For (i, mid+1, r) printf("%d: %ld %ld, %ld %ld\n", i, g0[i][0], g0[i][1], g1[i][0], g1[i][1]);
    int64_t mn[2] = {1ll << 60, 1ll << 60};
    int now = r;
    roF (i, min(mid, r-k+1), l) {
        while (now > mid && now - i + 1 >= k) {
            mn[0] = min(mn[0], g0[now][0]);
            mn[1] = min(mn[1], min(g0[now][0], g1[now][0]));
            --now;
        }
        // printf("Begin with %d, now=%d, calculated min(%ld+%ld, %ld+%ld)\n", i, now, f0[i][0], mn[1], f1[i][0], mn[0]);
        ans = min(ans, min(f0[i][0] + mn[1], f1[i][0] + mn[0]));
    }
}
inline void solveSingleTestCase() {
    int n;
    cin >> n >> k;
    For (i, 1, n) cin >> a[i];
    ans = 1ll << 60;
    solve(1, n);
    cout << ans << '\n';
    // puts("======");
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