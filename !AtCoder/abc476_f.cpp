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
#ifdef CLANGD
constexpr int N = 13;
#else
constexpr int N = 3005;
#endif

int n;
int64_t a[N], b[N], psum[N][N], ssum[N][N], mp[N][N], f[N][N], ys[N], pys[N], sys[N];
inline void solveSingleTestCase() {
    int M;
    cin >> n >> M;
    For(i, 1, n) cin >> a[i];
    For(i, 1, n) cin >> b[i];
    For(i, 1, n)
        For(j, 1, n) {
            int x = i + j;
            int y = i - j + n;
            mp[x][y] = a[i] * b[j] % M;
        }
    For(x, 1, n*2)
        For(y, 1, n*2)
            psum[x][y] = psum[x][y-1] + mp[x][y];
    For(x, 1, n*2)
        For(y, 1, n*2)
            psum[x][y] += psum[x][y-1];
    For(x, 1, n*2)
        roF(y, n*2, 1)
            ssum[x][y] = ssum[x][y+1] + mp[x][y];
    For(x, 1, n*2)
        roF(y, n*2, 1)
            ssum[x][y] += ssum[x][y+1];
    For(x, 1, n*2)
        For(y, 1, n*2)
            ys[x] += mp[x][y];
    For(x, 1, n*2) pys[x] = pys[x-1] + ys[x];
    roF(x, n*2, 1) sys[x] = sys[x+1] + ys[x];
    For(y, 1, n*2) {
        int64_t now = 0;
        For(x, 1, n*2) now += ys[x] * x + psum[x][y-1] + ssum[x][y+1];
        For(x, 1, n*2) {
            now -= sys[x]; now += pys[x-1];
            if (x + y - n & 1 ^ 1) {
                int i = (x+y-n)/2, j = (x-y+n)/2;
                if (1 <= i && i <= n && 1 <= j && j <= n) {
                    f[i][j] = now;
                }
            }
        }
    }
    int64_t ans = 0;
    For(i, 1, n)
        For(j, 1, n)
            ans ^= f[i][j] / 2 + (i-1) * n + (j-1);
    cout << ans;

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