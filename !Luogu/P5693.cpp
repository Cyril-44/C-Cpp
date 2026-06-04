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
        if (!dk || !db || (dk>0) ^ (db>0)) return INF;
        return db / dk;
    }
} l[N];
struct KTT {
    struct Node {
        Line lmx, rmx, mx, sum;
        int64_t cert;
        static inline void update(std::pair<Line, int64_t>&& res, Line& l, int64_t& c) { l = res.first; c = std::min(c, res.second); }
        friend Node operator+(const Node &x, const Node &y) {
            Node res {
                .lmx = x.lmx * (x.sum + y.lmx),
                .rmx = y.rmx * (y.sum + x.rmx),
                .sum = x.sum + y.sum,
                .cert = std::min(
                       x.lmx ^ (x.sum + y.lmx), 
                       y.rmx ^ (y.sum + x.rmx)
                )
            };
            res.mx = res.lmx * res.rmx * (x.rmx + y.lmx);
            res.cert = std::min({ res.cert,
                res.lmx ^ res.rmx, 
                res.lmx ^ (x.rmx + y.lmx), 
                res.rmx ^ (x.rmx + y.lmx)
            });
            return res;
        }
        void set(int64_t v) { lmx = rmx = mx = sum = {.k = 1, .b = v}, cert = INF; }
        void apply(int64_t v) { lmx+=v, rmx+=v, mx+=v, sum+=v; }
    } tr[N << 2];
    void apply(int u, int l, int r, int add) {
        if (tr[u].cert >= add) return tr[u].apply(add);
        int mid = l + r >> 1; // 证书过期，暴力重构
        apply(u<<1, l, mid, add);
        apply(u<<1|1, mid+1, r, add);
        tr[u] = tr[u<<1] + tr[u<<1|1];
    }
    int L, R;
    void pushdown(int u) {

    }
    void update(int u, int l, int r, int add) {
        if (L <= l && r <= R) apply(u, l, r, add);
        int mid = l + r >> 1;
        pushdown(u);
        if (L <= mid) update(u<<1, l, mid, add);
        if (mid < R) update(u<<1|1, mid+1, r, add);
        
    }
};
int main() {
    std::cin.tie(nullptr) -> sync_with_stdio(false);
    int q;
    std::cin >> n >> q;
    for (int i = 1; i <= n; i++) std::cin >> a[i];
    for (int op, l, r, x; q--; ) {
        std::cin >> op >> l >> r;
        if (op == 1) {
            std::cin >> x;

        } else {

        }
    }
    return 0;
}