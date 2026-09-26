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
int l[N], r[N];
struct BIT {
    int64_t t1[N], t2[N], n;

    int lowbit(int x) { return x & (-x); }

    void add(int k, int64_t v) {
        int64_t v1 = k * v;
        while (k <= n) {
            t1[k] += v, t2[k] += v1;
            // 注意不能写成 t2[k] += k * v，因为 k 的值已经不是原数组的下标了
            k += lowbit(k);
        }
    }

    int64_t getsum(int64_t *t, int64_t k) {
        int64_t ret = 0;
        while (k) {
            ret += t[k];
            k -= lowbit(k);
        }
        return ret;
    }

    void add1(int l, int r, int64_t v) {
        add(l, v), add(r + 1, -v);  // 将区间加差分为两个前缀加
    }

    long long getsum1(int l, int r) {
        return (r + 1ll) * getsum(t1, r) - 1ll * l * getsum(t1, l - 1) -
            (getsum(t2, r) - getsum(t2, l - 1));
    }
} f;
struct Query {
    int x, l, r, f, id;
    bool operator<(const Query& o) const {
        return x < o.x;
    }
} que[N*2];
int64_t ans[N];
inline void solveSingleTestCase() {
    int n, m, q;
    cin >> n >> m >> q;
    For(i, 1, n) cin >> l[i] >> r[i];
    For(i, 1, q) {
        int a, b, c, d;
        cin >> a >> b >> c >> d;
        que[i*2-1] = {a-1, c, d, -1, i};
        que[i*2] = {b, c, d, 1, i};
    }
    sort(que+1, que+1+q*2);
    int idx = 1;
    while (idx <= q*2 && que[idx].x == 0) ++idx;
    f.n = m;
    For(i, 1, n) {
        f.add1(l[i], r[i], 1);
        for (; idx <= q*2 && que[idx].x == i; idx++)
            ans[que[idx].id] += que[idx].f * f.getsum1(que[idx].l, que[idx].r);
    }
    For(i, 1, q)
        cout << ans[i] << '\n';
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