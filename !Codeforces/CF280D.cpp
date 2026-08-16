#include <bits/stdc++.h>
constexpr int N = 100005, K = 25;
int n, a[N], hist[K][2];
class SegTr {
    struct Range {
        int l, r, val;
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
    using MxPak = Pak<std::greater<int>>;
    struct Node {
        Pak<std::less<int>> mn;
        Pak<std::greater<int>> mx;
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
    int L, R, P, X;
    void updinv(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u].invert();
        int mid = l + r >> 1;
        pushdown(u);
        if (L <= mid) updinv(u<<1, l, mid);
        if (mid < R) updinv(u<<1|1, mid+1, r);
        pushup(u);
    }
    void updset(int u, int l, int r) {
        if (l == r) return tr[u].set(P, X);
        int mid = l + r >> 1;
        pushdown(u);
        if (P <= mid) updset(u<<1, l, mid);
        else updset(u<<1|1, mid+1, r);
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
    void apply(int pos, int val) { P=pos, X=val; updset(1, 1, n); }
    void invert(int l, int r) { L=l, R=r; updinv(1, 1, n); }
    MxPak inquire(int l, int r) { L=l, R=r; return inq(1, 1, n); }
} f;
int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
    f.build();
    int m;
    scanf("%d", &m);
    while (m--) {
        int op, x, y, z;
        scanf("%d%d%d", &op, &x, &y);
        if (op == 0) {
            f.apply(x, y);
        } else {
            scanf("%d", &z);
            int ans = 0, overallAns = 0;
            for (int i = 1; i <= z; i++) {
                auto res = f.inquire(x, y);
                ans += res.all.val;
                f.invert(res.all.l, res.all.r);
                hist[i][0] = res.all.l, hist[i][1] = res.all.r;
                overallAns = std::max(overallAns, ans);
            }
            for (int i = z; i >= 1; i--)
                f.invert(hist[i][0], hist[i][1]);
            printf("%d\n", overallAns);
        }
    }
    return 0;
}