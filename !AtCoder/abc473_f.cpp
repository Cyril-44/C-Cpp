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
constexpr int N = 55;
#else
constexpr int N = 500005;
#endif
char s[N];
int sum[N];
int n;
struct SegTr {
    struct Node {
        int min, add;
        void pull(int a) { min += a, add += a; }
    } tr[N << 2];
    int L, R, X;
    void pushdown(int u) {
        if (tr[u].add) {
            tr[u<<1].pull(tr[u].add);
            tr[u<<1|1].pull(tr[u].add);
            tr[u].add = 0;
        }
    }
    void pushup(int u) {
        tr[u].min = min(tr[u<<1].min, tr[u<<1|1].min);
    }
    void upd(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u].pull(X);
        int mid = l + r >> 1; pushdown(u);
        if (L <= mid) upd(u<<1, l, mid);
        if (mid < R) upd(u<<1|1, mid+1, r);
        pushup(u);
    }
    int inq(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u].min;
        int mid = l + r >> 1, res = 0x7fffffff; pushdown(u);
        if (L <= mid) res = min(res, inq(u<<1, l, mid));
        if (mid < R) res = min(res, inq(u<<1|1, mid+1, r));
        return res;
    }
    void update(int l, int x) { L=l, R=n, X=x; upd(1, 0, n); }
    int inquire(int l, int r) { L=l, R=r; return inq(1, 0, n); }
    void build(int u=1, int l=0, int r=n) {
        if (l == r) { tr[u].min = sum[l]; }
        else {
            int mid = l + r >> 1;
            build(u<<1, l, mid);
            build(u<<1|1, mid+1, r);
            pushup(u);
        }
    }
} f;
inline void solveSingleTestCase() {
    int q;
    cin >> n >> s >> q;
    roF(i, n, 1) s[i] = s[i-1];
    For(i, 1, n) {
        sum[i] = sum[i-1] + (s[i] == 'A' ? 1 : -1);
    }
    s[0] = 0;
    f.build();
    Rep(q) {
        int op, x;
        cin >> op >> x;
        if (op == 1) {
            char ch; cin >> ch;
            if (s[x] != ch) {
                if (ch == 'A') f.update(x, 2);
                else f.update(x, -2);
                s[x] = ch;
            }
        } else {
            int y; cin >> y;
            YESNO(f.inquire(x, y) >= f.inquire(x-1,x-1));
        }
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