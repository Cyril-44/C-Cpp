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

constexpr int N = 505;
char s[N][N];
int ps[N][N], sm[N], l0[N], r0[N], cnt[N];
inline void solveSingleTestCase() {
    int n, m, k;
    scanf("%d%d%d", &n, &m, &k);
    For(i, 1, n) scanf("%s", s[i]+1);
    // For(i, 1, n) For(j, 1, m)
    //     ps[i][j] = -ps[i-1][j-1] + ps[i-1][j] + ps[i][j-1] + (s[i][j] ^ '0');
    int64_t ans = 0;
    // For(il, 1, n) For(ir, il, n)
    //     For(jl, 1, m) For(jr, jl, m)
    //         if (ps[ir][jr] + ps[il-1][jl-1] - ps[ir][jl-1] - ps[il-1][jr] == k) ans++;
    // cout << ans << endl;
    // return ;
    For(i, 1, n) For(j, 1, m) ps[i][j] = ps[i][j-1] + (s[i][j] ^ '0');
    For(jl, 1, m) For(jr, jl, m) {
        int il = 1, ir = 1;
        int sum = 0;
        l0[1] = 1;
        For(i, 1, n) {
            sm[i] = ps[i][jr] - ps[i][jl-1];
            // printf("[%d,%d] sm[%d]=%d\n", lj, rj, i, sm[i]);
            if (sm[i]) l0[i+1] = i+1;
            else l0[i+1] = l0[i];
        }
        r0[n] = n;
        roF(i, n, 1) {
            if (sm[i]) r0[i-1] = i-1;
            else r0[i-1] = r0[i];
        }
        // putchar('*');for (int i = 1; i <= n; i++) printf("%d%c", sm[i], " \n"[i==n]);
        // putchar('$');for (int i = 1; i <= n; i++) printf("%d%c", l0[i], " \n"[i==n]);
        // putchar('>');for (int i = 1; i <= n; i++) printf("%d%c", r0[i], " \n"[i==n]);
        while (il <= n) {
            while (ir <= n && sum < k || ir <= il) sum += sm[ir++];
            if (sum == k) {
                ans += (r0[ir-1] - (ir-1) + 1ll);
                // printf("%d [%d,%d] [%d,%d] %lld\n", sum, il, ir-1, jl, jr, (r0[ir-1] - (ir-1) + 1ll));
            }
            sum -= sm[il++];
        }
    }
    cout << ans << '\n';
}
int main() {
    // cin.tie(nullptr) -> sync_with_stdio(false);
    int testCases = 1;
#ifdef MULTI_TEST_CASES
    cin >> testCases;
#endif
    while (testCases--) solveSingleTestCase();
    return 0;
}