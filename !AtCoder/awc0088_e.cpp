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

constexpr int N = 50005;
struct Query {
    int l, r, lid, rid, id;
    friend bool operator<(const Query &x, const Query &y) { return x.lid < y.lid || x.lid == y.lid && (x.lid & 1 ? x.rid > y.rid : x.rid < y.rid); }
} q[N];
char s[N];
int sum[N];
int64_t ans[N];
struct MoImpl {
    int _cntl[N << 1], _cntr[N << 1], *cntl = _cntl + N, *cntr = _cntr + N;
    int64_t ans;
    void addval(int id) { ++cntl[sum[id-1]], ++cntr[sum[id]]; }
    void delval(int id) { --cntl[sum[id-1]], --cntr[sum[id]]; }
    void addlval(int id) {
        addval(id);
        ans += cntr[sum[id-1]-1] + cntr[sum[id-1]] + cntr[sum[id-1]+1];
    }
    void addrval(int id) {
        addval(id);
        ans += cntl[sum[id]-1] + cntl[sum[id]] + cntl[sum[id]+1];
    }
    void dellval(int id) {
        ans -= cntr[sum[id-1]-1] + cntr[sum[id-1]] + cntr[sum[id-1]+1];
        delval(id);
    }
    void delrval(int id) {
        ans -= cntl[sum[id]-1] + cntl[sum[id]] + cntl[sum[id]+1];
        delval(id);
    }
    int l = 1, r = 0;
    void update(int ql, int qr) {
        while (l > ql) addlval(--l);
        while (r < qr) addrval(++r);
        while (l < ql) dellval(l++);
        while (r > qr) delrval(r--);
    }
    int64_t inquire() { return ans; }
} mo;
inline void solveSingleTestCase() {
    int n, m;
    cin >> n >> m >> s;
    For (i, 1, n) sum[i] = sum[i-1] + (s[i-1] == 'W' ? 1 : -1);
    int B = sqrt(n);
    For (i, 1, m) {
        cin >> q[i].l >> q[i].r;
        q[i].lid = q[i].l / B;
        q[i].rid = q[i].r / B;
        q[i].id = i;
    }
    sort(q+1, q+1+m);
    For (i, 1, m) {
        mo.update(q[i].l, q[i].r);
        ans[q[i].id] = mo.inquire();
    }
    For (i, 1, m) cout << ans[i] << '\n';
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