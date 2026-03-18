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

constexpr int N = 5005, MOD = 998244353;
int f[N], x[N], nxt[N];
/*
这题的 x 实际上是一个类似于单调栈的东西，里面每一个区间 [xi, i] 都是互相包含或者不相交的。
考虑对于这种区间分层考虑。如果当前处理区间 [xi, i] 那么就将**包含在其中**所有的极长互不相交的区间拎出来，他们是单调递增的。
f[i] 表示当前状态下，单调栈中有 i 个元素的时候 x 中 -1 的选法
于是如果，在这些区间除外的地方，有着一些空位（xj = -1），那么这些空位就可以以一个后缀和的形式转移到 f 中去，具体来说，这个区间可以将左端点设置在之前任何一个区间的开始位置，对应的弹出栈中元素。
转移就是 f[i] --> f[i+1] 或者 sufSumf[i] --> f[i+1] （都要求 i 倒序枚举）
*/
inline void solveSingleTestCase() {
    int n;
    cin >> n;
    For (i, 1, n) cin >> x[i];
    For (i, 1, n) { // 先判断区间重叠 ==> 无解
        if (x[i] >= i) return void(cout << "0\n");
        For (j, i+1, n)
            if (~x[i] && ~x[j] && x[i] < x[j] && x[j] < i) return void(cout << "0\n");
    }
    memset(nxt, 0, sizeof(int) * (n+1));
    vector<pii_t> ranges; ranges.reserve(n+1);
    For (i, 1, n) if (~x[i]) ranges.emplace_back(x[i], i);
    sort(ranges.begin(), ranges.end(), [](const pii_t&x, const pii_t&y){return x.second - x.first < y.second - y.first;});
    ranges.emplace_back(0, n+1);
    int ans = 1;
    for (auto [l, r] : ranges) { // 对每一个区间单独 DP
        int cnt = 0; // 到当前为止，极长区间的数量
        int bl = l;
        f[0] = 1;
        while (l < r) {
            bool free = nxt[l] ? (l = nxt[l], false) : (l++, true);
            if (l >= r) break;
            ++cnt;
            if (free) { roF (i, cnt - 1, 1) (f[i-1] += f[i]) %= MOD; roF (i, cnt, 1) f[i] = f[i-1]; f[0] = 0; }
            else { roF (i, cnt, 1) f[i] = f[i-1]; f[0] = 0; }
            // fprintf(stderr, "Cur l=%d,free=%d: ", l, (int)free);
            // For (i, 0, cnt) fprintf(stderr, "%d ", f[i]);
            // fprintf(stderr, "\n");
        }
        nxt[bl] = r;
        int sum = 0;
        For (i, 0, cnt) (sum += f[i]) %= MOD;
        // fprintf(stderr, "Range[%d, %d] value: %d\n", bl, r, sum);
        ans = 1ull * ans * sum % MOD;
    }
    cout << ans << '\n';
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