#include <bits/stdc++.h>
constexpr int N = 200005, K = 25;
int n, a[N], hist[K][2];
class SegTr {
    struct Range {
        int l, r;
        int64_t val;
        friend Range operator+(const Range& x, const Range& y) { return {x.l, y.r, x.val + y.val}; }
        void swap(Range& o) {
            val = -val, o.val = -o.val;
            std::swap(*this, o);
        }
    };
    template<class Comp> struct Pak {
        Range pre, suf, all, sum;
        static void update(Range& x, const Range& y) { if (Comp{}(y.val, x.val)) x = y; }
        friend Pak operator+(const Pak& l, const Pak& r) {
            Pak pak{ l.sum + r.pre, l.suf + r.sum, l.suf + r.pre, l.sum + r.sum };
            update(pak.pre, l.pre), update(pak.suf, r.suf);
            update(pak.all, l.all), update(pak.all, r.all);
            return pak;
        }
    };
    using MxPak = Pak<std::greater<int64_t>>;
    struct Node {
        Pak<std::less<int64_t>> mn;
        MxPak mx;
        bool inv; // 符号取反
        void set(int pos, int val) {
            mn = {{pos,pos,val},{pos,pos,val},{pos,pos,val},{pos,pos,val}};
            mx = {{pos,pos,val},{pos,pos,val},{pos,pos,val},{pos,pos,val}};
        }
        void invert() {
            inv ^= 1;
            mn.pre.swap(mx.pre);
            mn.suf.swap(mx.suf);
            mn.all.swap(mx.all);
            mn.sum.swap(mx.sum);            
        }
    } tr[N << 2];
    void pushup(int u) {
        tr[u].mn = tr[u<<1].mn + tr[u<<1|1].mn;
        tr[u].mx = tr[u<<1].mx + tr[u<<1|1].mx;
    }
    void pushdown(int u) {
        if (tr[u].inv) {
            tr[u<<1].invert();
            tr[u<<1|1].invert();
            tr[u].inv = false;
        }
    }
    int L, R;
    void upd(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u].invert();
        int mid = l + r >> 1;
        pushdown(u);
        if (L <= mid) upd(u<<1, l, mid);
        if (mid < R) upd(u<<1|1, mid+1, r);
        pushup(u);
    }
    MxPak inq(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u].mx;
        int mid = l + r >> 1;
        pushdown(u);
        if (R <= mid) return inq(u<<1, l, mid);
        if (mid < L) return inq(u<<1|1, mid+1, r);
        return inq(u<<1, l, mid) + inq(u<<1|1, mid+1, r);
    }
public:
    void build(int u=1, int l=1, int r=n) {
        if (l == r) return tr[u].set(l, a[l]);
        int mid = l + r >> 1;
        build(u<<1, l, mid);
        build(u<<1|1, mid+1, r);
        pushup(u);
    }
    void invert(int l, int r) { L=l, R=r; upd(1, 1, n); }
    MxPak inquire(int l, int r) { L=l, R=r; return inq(1, 1, n); }
} f;
int main() {
    scanf("%d", &n);
    int64_t sum1 = 0;
    for (int i = 1; i <= n; i++) {
        int flg;
        scanf("%d%d", &flg, &a[i]);
        if (flg) sum1 += a[i], a[i] = -a[i];
    }
    f.build();
    auto res = f.inquire(1, n);
    int m;
    scanf("%d", &m);
    int64_t sum = 0;
    if (res.pre.val > 0) sum += res.pre.val, f.invert(res.pre.l, res.pre.r);
    printf("%lld\n", sum + sum1);
    for (int i = 1; i <= m; i++) {
        res = f.inquire(1, n);
        if (res.all.val > 0) sum += res.all.val, f.invert(res.all.l, res.all.r);
        printf("%lld\n", sum + sum1);
    }
    return 0;
}