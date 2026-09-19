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
int n;
struct SegTr {
    struct Node {
        int mx, mxid, mn, mnid;
        Node& operator+=(const Node &o) {
            if (o.mx > mx) mx=o.mx, mxid=o.mxid;
            if (o.mn < mn) mn=o.mn, mnid=o.mnid;
            return *this;
        }
        friend Node operator+(Node x, const Node &y) { return x += y; }
    } tr[N << 2];
    void pushup(int u) {
        tr[u] = tr[u<<1] + tr[u<<1|1];
    }
    int L, R, P, X;
    Node inq(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u];
        int mid = l + r >> 1;
        if (R <= mid) return inq(u<<1, l, mid);
        if (L > mid) return inq(u<<1|1, mid+1, r);
        return inq(u<<1, l, mid) + inq(u<<1|1, mid+1, r);
    }
    void upd(int u, int l, int r) {
        if (l == r) tr[u] = {X,P,X,P};
        else {
            int mid = l + r >> 1;
            if (P <= mid) upd(u<<1, l, mid);
            else upd(u<<1|1, mid+1, r);
            pushup(u);
        }
    }
    void update(int p, int x) { P=p, X=x; upd(1, 1, n); }
    Node inquire(int l, int r) { L=l, R=r; return inq(1, 1, n); }
    void print(int u=1, int l=1, int r=n) {
        if (l==r) cout << tr[u].mx << ' ';
        else {
            int mid = l + r >> 1;
            print(u<<1, l, mid);
            print(u<<1|1, mid+1, r);
        }
    }
    void build(int u=1, int l=1, int r=n) {
        if (l == r) {
            int x; cin >> x;
            tr[u] = {x,l,x,l};
        } else {
            int mid = l + r >> 1;
            build(u<<1, l, mid);
            build(u<<1|1, mid+1, r);
            pushup(u);
        }
    }
} f;
inline void solveSingleTestCase() {
    int m;
    cin >> n >> m;
    f.build();
    Repv(m, l, r) {
        cin >> l >> r;
        auto res = f.inquire(l, r);
        f.update(res.mnid, res.mx);
        f.update(res.mxid, res.mn);
    }
    f.print();
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