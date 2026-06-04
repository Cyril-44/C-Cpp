#include <cstdio>
#include <cstdint>
#include <iostream>
#include <algorithm>
constexpr int N = 400005;
constexpr int64_t INF = 1ll<<60;
int n, a[N];
struct Line {
    int64_t b; int k;
    Line& operator+=(const Line& o) { b+=o.b, k+=o.k; return *this; }
    friend Line operator+(Line p, const Line& q) { return p += q; }
    Line& operator+=(int64_t o) { b += k * o; return *this; } // 将当前坐标系向右平移 o 个单位
    friend Line operator*(const Line& p, const Line& q) { // 返回零点更大的，且上升趋势更大的直线
        if (p.b < q.b) return q;
        if (q.b < p.b) return p;
        if (p.k < q.k) return q;
        return p;
    }
    friend int64_t operator^(const Line& p, const Line& q) {
        int dk = p.k - q.k; int64_t db = q.b - p.b;
        if (!dk || !db || ((dk>0) ^ (db>0))) return INF;
        return db / dk;
    }
};
struct KTT {
    struct Node {
        Line lmx, rmx, mx, sum;
        int64_t cert = INF, add = 0;
        static inline void update(std::pair<Line, int64_t>&& res, Line& l, int64_t& c) { l = res.first; c = std::min(c, res.second); }
        friend Node operator+(const Node &x, const Node &y) { return {
            .lmx = x.lmx * (x.sum + y.lmx),
            .rmx = y.rmx * (y.sum + x.rmx),
            .mx = x.mx * y.mx * (x.rmx + y.lmx),
            .sum = x.sum + y.sum,
            .cert = std::min({
                x.cert, y.cert, // 不要忘了继承证书！！
                x.lmx ^ (x.sum + y.lmx), 
                y.rmx ^ (y.sum + x.rmx),
                x.mx ^ y.mx,
                x.mx ^ (x.rmx + y.lmx),
                y.mx ^ (x.rmx + y.lmx)
            })
        }; }
        void set(int64_t v) { lmx = rmx = mx = sum = {.b = v, .k = 1}; }
        void apply(int64_t v) { lmx+=v, rmx+=v, mx+=v, sum+=v, add+=v, cert-=v; }
    } tr[N << 2];
    void apply(int u, int l, int r, int64_t add) {
        if (tr[u].cert >= add) return tr[u].apply(add);
        int mid = l + r >> 1; // 证书过期，暴力重构
        apply(u<<1, l, mid, add + tr[u].add); // 千万不要忘了懒标记！！！
        apply(u<<1|1, mid+1, r, add + tr[u].add);
        tr[u] = tr[u<<1] + tr[u<<1|1];
    }
    int L, R;
    void build(int u=1, int l=1, int r=n) {
        if (l == r) return tr[u].set(a[l]);
        int mid = l + r >> 1;
        build(u<<1, l, mid), build(u<<1|1, mid+1, r);
        tr[u] = tr[u<<1] + tr[u<<1|1];
    }
    void pushdown(int u, int l, int r) {
        int mid = l + r >> 1;
        if (tr[u].add) {
            apply(u<<1, l, mid, tr[u].add);
            apply(u<<1|1, mid+1, r, tr[u].add);
            tr[u].add = 0;
        }
    }
    void upd(int u, int l, int r, int add) {
        if (L <= l && r <= R) return apply(u, l, r, add);
        int mid = l + r >> 1;
        pushdown(u, l, r);
        if (L <= mid) upd(u<<1, l, mid, add);
        if (mid < R) upd(u<<1|1, mid+1, r, add);
        tr[u] = tr[u<<1] + tr[u<<1|1];
    }
    Node inq(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u];
        int mid = l + r >> 1;
        pushdown(u, l, r);
        if (R <= mid) return inq(u<<1, l, mid);
        if (mid < L) return inq(u<<1|1, mid+1, r);
        return inq(u<<1, l, mid) + inq(u<<1|1, mid+1, r);
    }
    void update(int l, int r, int x) { L=l, R=r; upd(1,1,n,x); }
    int64_t inquire(int l, int r) { L=l, R=r; return inq(1,1,n).mx.b; }
} fs;
int main() {
    std::cin.tie(nullptr) -> sync_with_stdio(false);
    int q;
    std::cin >> n >> q;
    for (int i = 1; i <= n; i++) std::cin >> a[i];
    fs.build();
    for (int op, l, r, x; q--; ) {
        std::cin >> op >> l >> r;
        if (op == 1) {
            std::cin >> x;
            fs.update(l, r, x);
        } else {
            printf("%lld\n", std::max((int64_t)0, fs.inquire(l, r)));
        }
    }
    return 0;
}