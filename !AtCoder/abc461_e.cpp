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
int rlast[N], clast[N], cvec[N], rvec[N];
struct BIT {
    std::vector<int> tr;
    void upd(int p, int x) {
        for (; p < (int)tr.size(); p += p & -p) tr[p] += x;
    }
    int sum(int p) {
        int res = 0;
        for (; p > 0; p -= p & -p) res += tr[p];
        return res;
    }
    BIT(int n) : tr(n+1) {}
};
enum OPType {ROW, COL, FIRSTROW};
inline void solveSingleTestCase() {
    int n, q;
    cin >> n >> q;
    int topc = 0, topr = 0;
    memset(rlast, -1, sizeof(int) * (n+1));
    vector<tuple<OPType,int,int>> ques;
    Repv(q, op, x) {
        cin >> op >> x;
        switch (op) {
        case 1: // Row x ==> Black
            // 在 allc 中查询 [lr[x], topc] 不同颜色数量
            if (rlast[x] == -1) ques.emplace_back(FIRSTROW, -1, -1), rlast[x] = 0;
            else ques.emplace_back(COL, rlast[x]+1, topc);
            rlast[x] = topc, rvec[++topr] = x;
        break;
        case 2: // Col x ==> White
            // 在 allr 中查询 [lc[x], topr] 不同颜色数量
            ques.emplace_back(ROW, clast[x]+1, topr);
            clast[x] = topr, cvec[++topc] = x;
        }
    }
    memset(rlast, -1, sizeof(int) * (n+1));
    memset(clast, -1, sizeof(int) * (n+1));
    BIT rfs(topr), cfs(topc);
    int rnow = 0, cnow = 0;
    int64_t ans = 0;
    for (auto [tp, L, R] : ques) { // 在 ar 上处理询问
        switch (tp) {
        case FIRSTROW: ans += n; break;
        case COL:
            while (cnow < R) {
                int col = cvec[++cnow];
                if (~clast[col]) cfs.upd(clast[col], -1);
                cfs.upd(clast[col] = cnow, 1);
            }
            ans += cfs.sum(R) - cfs.sum(L-1);
        break;
        case ROW:
            while (rnow < R) {
                int col = rvec[++rnow];
                if (~rlast[col]) rfs.upd(rlast[col], -1);
                rfs.upd(rlast[col] = rnow, 1);
            }
            ans -= rfs.sum(R) - rfs.sum(L-1);
        }
        cout << ans << '\n';
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