#include <cstdio>
#include <algorithm>
constexpr int N = 100005, NX = 39989;
constexpr double EPS = 1e-9;
struct Frac {
    __int128 num = 0, den = 1;
    [[gnu::always_inline]] inline Frac operator-() const { return Frac(-num, den); }
    [[gnu::always_inline]] inline friend Frac operator+(const Frac &a, const Frac &b) { return {a.num * b.den + a.den * b.num, a.den * b.den}; }
    [[gnu::always_inline]] inline friend Frac operator-(const Frac &a, const Frac &b) { return a + -b; }
    [[gnu::always_inline]] inline friend Frac operator*(const Frac &a, const Frac &b) { return {a.num * b.num, a.den * b.den}; }
    [[gnu::always_inline]] inline friend bool operator<(const Frac &a, const Frac &b) { return a.num * b.den < a.den * b.num; }
    [[gnu::always_inline]] inline friend bool operator>(const Frac &a, const Frac &b) { return b < a; }
    [[gnu::always_inline]] inline friend bool operator==(const Frac &a, const Frac &b) { return a.num * b.den == a.den * b.num; }
    [[gnu::always_inline]] inline friend bool operator!=(const Frac &a, const Frac &b) { return !(a == b); }
};
struct Line {
    Frac k, b;
    int id;
    inline Frac operator()(int x) const { return k * Frac{1l*x} + b; }
};
struct SegSegTr {
    Line tr[NX+1 << 2];
    inline bool gt(const Line &a, const Line &b, int x) const { return a(x) == b(x) && a.id < b.id || a(x) > b(x); }
    inline bool lt(const Line &a, const Line &b, int x) const { return gt(b, a, x); }
    inline void tominmax(Line &a, Line &b, int x) const { if (gt(a, b, x)) std::swap(a, b); }
    inline Line max(const Line &a, const Line &b, int x) const { return gt(a, b, x) ? a : b; }
    void pull(int u, int l, int r, Line s) {
        if (l == r) return tominmax(s, tr[u], l);
        int mid = l + r >> 1;
        tominmax(s, tr[u], mid); // 维护一个最高的中点，因为当前区间原来的线段和新来的线段一定有一个比另外一个更优（也就是在更多的区域比另外一条线段高）
        if (gt(s, tr[u], l)) pull(u<<1, l, mid, s); // s 可能在左边更优
        else if (gt(s, tr[u], r)) pull(u<<1|1, mid+1, r, s); // s 可能在右边更优
    }
    int L, R; Line S;
    void upd(int u, int l, int r) {
        if (L <= l && r <= R) return pull(u, l, r, S);
        int mid = l + r >> 1;
        if (L <= mid) upd(u<<1, l, mid);
        if (mid < R) upd(u<<1|1, mid+1, r);
    }
    inline void update(int l, int r, const Line& s) { L=l, R=r, S=s; upd(1, 1, NX); }
    inline void insert(int x0, int y0, int x1, int y1, int id) {
        if (x0 == x1) return update(x0, x0, {Frac(0), Frac(std::max(y0, y1)), id});
        if (x0 > x1) std::swap(x0, x1), std::swap(y0, y1);
        update(x0, x1, {Frac(y1-y0, x1-x0), Frac(1l*x1*y0 - 1l*x0*y1, x1-x0), id});
    }
    inline Line inquire(int pos) const {
        int u = 1, l = 1, r = NX;
        Line res = tr[u];
        while (l != r) {
            int mid = l + r >> 1;
            if (pos <= mid) u=u<<1, r=mid;
            else u=u<<1|1, l=mid+1;
            res = max(res, tr[u], pos);
        }
        return res;
    }
} lcf;
int main() {
    int n, lastans = 0, cnt = 0;
    scanf("%d", &n);
    for (int _n = 0; _n < n; _n++) {
        int op;
        scanf("%d", &op);
        if (op == 0) {
            int k;
            scanf("%d", &k);
            k = (k + lastans - 1) % NX + 1;
            printf("%d\n", lastans = lcf.inquire(k).id);
        } else {
            int x0, x1, y0, y1;
            scanf("%d%d%d%d", &x0, &y0, &x1, &y1);
            x0 = (x0 + lastans - 1) % NX + 1;
            x1 = (x1 + lastans - 1) % NX + 1;
            y0 = (y0 + lastans - 1) % (int)1e9 + 1;
            y1 = (y1 + lastans - 1) % (int)1e9 + 1;
            lcf.insert(x0, y0, x1, y1, ++cnt);
        }
    }
    return 0;
}