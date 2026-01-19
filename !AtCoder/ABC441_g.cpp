#include <bits/stdc++.h>
#include <cassert>
// #define LUOGU
#if defined(ONLINE_JUDGE) && !defined(LUOGU)
#pragma GCC optimize(2, 3, "inline", "unroll-loops", "fast-math", "inline-small-functions", "no-stack-protector",      \
                     "delete-null-pointer-checks")
#pragma GCC target("tune=native")
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
using namespace std;
using pii_t = pair<int, int>;
using pll_t = pair<int64_t, int64_t>;
using veci_t = vector<int>;
using vecl_t = vector<int64_t>;
Inline int Popcnt(int x) {
    return __builtin_popcount((unsigned)x);
}
Inline int Popcnt(unsigned x) {
    return __builtin_popcount(x);
}
Inline int Popcnt(int64_t x) {
    return __builtin_popcountll((uint64_t)x);
}
Inline int Popcnt(uint64_t x) {
    return __builtin_popcountll(x);
}
Inline int Log2(int x) {
    return 31 - __builtin_clz((unsigned)x | 1);
}
Inline int Log2(unsigned x) {
    return 31 - __builtin_clz(x | 1);
}
Inline int Log2(int64_t x) {
    return 63 - __builtin_clzll((uint64_t)x | 1);
}
Inline int Log2(uint64_t x) {
    return 63 - __builtin_clzll(x | 1);
}

// #define MULTI_TEST_CASES

constexpr int N = 200005;

struct SegTree {
    SegTree(int n_) : n(n_), tr(n_ + 1 << 2) {}
    inline int64_t inquire(int l, int r) {
        L = l, R = r;
        return que(1, 1, n);
    }
    inline void plus(int l, int r, int x) {
        L = l, R = r, X = x;
        upd1(1, 1, n);
    }
    inline void flip(int l, int r) {
        L = l, R = r;
        upd2(1, 1, n);
    }

  private:
    int n;
    struct Node {
        int64_t mx, add;
        bool off, flp;
        Node() : mx(), add(), off(), flp() {}
        inline void pull(int64_t ad, bool fl) {
            if (fl)
                off ^= 1, flp ^= 1, add = mx = 0;
            else if (!off)
                add += ad, mx += ad;
        }
    };
    vector<Node> tr;
    int L, R, X;
    inline void pushup(int u) {
        tr[u].mx = max(tr[u << 1].mx, tr[u << 1 | 1].mx);
        assert(!(tr[u << 1].off & tr[u << 1 | 1].off));
    }
    inline void pushdown(int u) {
        tr[u << 1].pull(tr[u].add, tr[u].flp);
        tr[u << 1 | 1].pull(tr[u].add, tr[u].flp);
        tr[u].add = 0, tr[u].flp = 0;
    }
    inline int64_t que(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u].mx;
        int mid = l + r >> 1;
        int64_t res = 0;
        pushdown(u);
        if (L <= mid) res = max(res, que(u << 1, l, mid));
        if (mid < R) res = max(res, que(u << 1 | 1, mid + 1, r));
        printf("que %d[%d~%d]: %lld %d res=%lld\n", u, l, r, tr[u].mx, (int)tr[u].flp, res);
        return res;
    }
    inline void upd1(int u, int l, int r) {
        if (tr[u].off) return;
        if (L <= l && r <= R)
            tr[u].pull(X, false);
        else {
            int mid = l + r >> 1;
            pushdown(u);
            if (L <= mid) upd1(u << 1, l, mid);
            if (mid < R) upd1(u << 1 | 1, mid + 1, r);
            pushup(u);
        }
        fprintf(stderr, "upd1 %d[%d~%d]: mx=%lld%s\n", u, l, r, tr[u].mx, tr[u].off ? ", off" : "");
    }
    inline void upd2(int u, int l, int r) {
        if (L <= l && r <= R)
            tr[u].pull(0, true);
        else {
            int mid = l + r >> 1;
            pushdown(u);
            if (L <= mid) upd2(u << 1, l, mid);
            if (mid < R) upd2(u << 1 | 1, mid + 1, r);
            pushup(u);
        }
        fprintf(stderr, "upd2 %d[%d~%d]: mx=%lld%s\n", u, l, r, tr[u].mx, tr[u].off ? ", off" : "");
    }
#undef DEF
};
inline void solveSingleTestCase() {
    int n, q;
    cin >> n >> q;
    SegTree f(n);
    for (int opt, l, r, x; q--;) {
        cin >> opt >> l >> r;
        fprintf(stderr, "? %d %d %d %d\n", opt, l, r, x);
        if (opt == 1) {
            cin >> x;
            f.plus(l, r, x);
        } else if (opt == 2) {
            f.flip(l, r);
        } else {
            cout << f.inquire(l, r) << '\n';
        }
    }
}
int main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    int testCases = 1;
#ifdef MULTI_TEST_CASES
    cin >> testCases;
#endif
    while (testCases--) solveSingleTestCase();
    return 0;
}