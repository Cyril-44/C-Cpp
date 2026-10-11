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
namespace Solution {
#define MULTI_TEST_CASES

constexpr int N = 105, INF = 0x3f3f3f3f;
int a[N], b[N], aa[N], ab[N], c[N][N];
int u[N], v[N], p[N], pre[N], d[N];
bool vis[N];

inline void globalInit() {}

inline int calc(int n, int k) {
    For(i, 1, n) For(j, 1, n)
        c[i][j] = ((a[i] ^ b[j] ^ k) & 1) ? INF :
                  b[j] == a[i] + k ? 0 :
                  abs(a[i] + b[j]) <= k ? 1 :
                  abs(a[i] - b[j]) <= k ? 2 : INF;

    memset(u, 0, sizeof u);
    memset(v, 0, sizeof v);
    memset(p, 0, sizeof p);

    For(i, 1, n) {
        p[0] = i;
        int j0 = 0;
        fill(d, d + n + 1, INF);
        memset(vis, 0, sizeof vis);
        do {
            vis[j0] = true;
            int i0 = p[j0], delta = INF, j1 = 0;
            For(j, 1, n) if (!vis[j]) {
                int w = c[i0][j] - u[i0] - v[j];
                if (w < d[j]) d[j] = w, pre[j] = j0;
                if (d[j] < delta) delta = d[j], j1 = j;
            }
            For(j, 0, n)
                if (vis[j]) u[p[j]] += delta, v[j] -= delta;
                else d[j] -= delta;
            j0 = j1;
        } while (p[j0]);

        do {
            int j1 = pre[j0];
            p[j0] = p[j1];
            j0 = j1;
        } while (j0);
    }
    return -v[0];
}

inline void solveSingleTestCase() {
    int n, pra[2]{}, prb[2]{};
    cin >> n;
    For(i, 1, n) cin >> a[i], aa[i] = abs(a[i]), pra[aa[i] & 1]++;
    For(i, 1, n) cin >> b[i], ab[i] = abs(b[i]), prb[ab[i] & 1]++;
    sort(aa + 1, aa + n + 1);
    sort(ab + 1, ab + n + 1);

    int mnk[2] = {INF, INF};
    For(q, 0, 1) {
        if (pra[0] != prb[q] || pra[1] != prb[q ^ 1]) continue;
        mnk[q] = 0;
        int j[2] = {1, 1};
        For(i, 1, n) {
            int r = (aa[i] & 1) ^ q;
            while ((ab[j[r]] & 1) != r) ++j[r];
            mnk[q] = max(mnk[q], abs(aa[i] - ab[j[r]++]));
        }
    }

    int ans = min(mnk[0], mnk[1]);
    if (ans == INF) EXIT(-1 << '\n');
    ans += 2 * n;
    For(q, 0, 1)
        for (int k = mnk[q]; k < ans; k += 2)
            ans = min(ans, k + calc(n, k));

    cout << ans << '\n';
}
}
int main() {
    cin.tie(nullptr) -> sync_with_stdio(false);
    int testCases = 1;
#ifdef MULTI_TEST_CASES
    cin >> testCases;
#endif
    while (testCases--) Solution::solveSingleTestCase();
    return 0;
}