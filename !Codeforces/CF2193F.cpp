#include <bits/stdc++.h>
// #define LUOGU
#if defined(ONLINE_JUDGE) && !defined(LUOGU)
# pragma GCC optimize(2, 3, "inline", "unroll-loops", "fast-math", "inline-small-functions", "no-stack-protector", "delete-null-pointer-checks")
# pragma GCC target("tune=native")
#endif
#define Inline __attribute__((always_inline)) inline
#define For(i, s, t) for (int i = (s); i <= (t); ++i)
#define Forv(i, s, t, ...) for (int i = (s), __VA_ARGS__; i <= (int)(t); ++i)
#define roF(i, t, s) for (int i = (t); i >= (s); --i)
#define roFv(i, t, s, ...) for (int i = (t), __VA_ARGS__; i >= (int)(s); --i)
#define Rep(c) for (int tempFor_count = c; tempFor_count; --tempFor_count)
#define Repv(c, ...) for (int tempFor_count = c, __VA_ARGS__; tempFor_count; --tempFor_count)
#define YES return cout << "Yes\n", void()
#define NO return cout << "No\n", void()
#define YESNO(j) cout << ((j) ? "Yes\n" : "No\n")
using namespace std;using pii_t=pair<int,int>;using pll_t=pair<int64_t,int64_t>;using veci_t=vector<int>;using vecl_t=vector<int64_t>;Inline int Popcnt(int x){return __builtin_popcount((unsigned)x);}Inline int Popcnt(unsigned x){return __builtin_popcount(x);}Inline int Popcnt(int64_t x){return __builtin_popcountll((uint64_t)x);}Inline int Popcnt(uint64_t x){return __builtin_popcountll(x);}Inline int Log2(int x){return 31-__builtin_clz((unsigned)x|1);}Inline int Log2(unsigned x){return 31-__builtin_clz(x|1);}Inline int Log2(int64_t x){return 63-__builtin_clzll((uint64_t)x|1);}Inline int Log2(uint64_t x){return 63-__builtin_clzll(x|1);}

#define MULTI_TEST_CASES

constexpr int N = 200005;
#define X first
#define Y second
inline void solveSingleTestCase() {
    int n;
    std::pair<int,int> A, B;
    cin >> n >> A.X >> A.Y >> B.X >> B.Y;
    std::vector<std::pair<int,int>> pnts(n);
    For (i, 0, n-1) cin >> pnts[i].X;
    For (i, 0, n-1) cin >> pnts[i].Y;
    map<int,pair<int,int>> mp;
    For (i, 0, n-1) {
        if (!mp.count(pnts[i].X)) mp.emplace(pnts[i].X, make_pair(numeric_limits<int>::max(), numeric_limits<int>::min()));
        auto &[l, r] = mp[pnts[i].X];
        l = min(l, pnts[i].Y), r = max(r, pnts[i].Y);
    }
    vector<pair<int,pair<int,int>>> vec; vec.reserve(mp.size()+1);
    vec.emplace_back(A.X, make_pair(A.Y, A.Y));
    for (const auto &i : mp) vec.push_back(i);
    vector<pair<int64_t,int64_t>> f(mp.size() + 1);
    For (i, 1, mp.size()) {
        const auto &[g0, g1] = f[i-1];
        auto &[f0, f1] = f[i];
        f0 = vec[i].X - vec[i-1].X + vec[i].Y.Y - vec[i].Y.X + min(g0 + abs(vec[i].Y.Y - vec[i-1].Y.X), g1 + abs(vec[i].Y.Y - vec[i-1].Y.Y));
        f1 = vec[i].X - vec[i-1].X + vec[i].Y.Y - vec[i].Y.X + min(g0 + abs(vec[i].Y.X - vec[i-1].Y.X), g1 + abs(vec[i].Y.X - vec[i-1].Y.Y));
    }
    const auto &[lx, ly] = vec[mp.size()];
    const auto &[f0, f1] = f[mp.size()];
    cout << B.X - lx + min(f0 + abs(B.Y - ly.X), f1 + abs(B.Y - ly.Y)) << '\n';
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