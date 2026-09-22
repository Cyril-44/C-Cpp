#include <cstdio>
#include <cmath>
#include <cstdint>
#include <algorithm>
template<class T> inline void in(T &x) {
    char ch = getchar();
    while (ch < '0' || ch > '9') ch = getchar();
    for (x = 0; ch >= '0' && ch <= '9'; ch = getchar())
        x = (x << 3) + (x << 1) + (ch ^ '0');
}
#ifdef CLANGD
constexpr int N = 10;
#else
constexpr int N = 100004;
#endif
constexpr uint64_t INF = -1ull;
int d[N], m[N];
struct LCTreeBase {
    struct Line {
        uint64_t b; unsigned k;
        Line(unsigned _k=0, uint64_t _b=0) : b(_b), k(_k) {}
        uint64_t operator()(int x) const { return 1ull * k * x + b; }
    } ln[N*20];
    int bin[N*20], ls[N*20], rs[N*20], top;
    inline int alloc() { // Ensure ln is set.
        if (bin[0]) {
            int id = bin[bin[0]--];
            ls[id] = rs[id] = 0;
            return id;
        }
        return ++top;
    }
    void upd(int &u, int l, int r, Line x) {
        if (!u) {
            ln[u = alloc()] = x;
            return;
        }
        if (l == r) {
            if (x(l) < ln[u](l)) ln[u] = x;
            return;
        }
        int mid = l + r >> 1;
        if (x(mid) < ln[u](mid)) std::swap(x, ln[u]);
        if (x(l) < ln[u](l)) upd(ls[u], l, mid, x);
        if (x(r) < ln[u](r)) upd(rs[u], mid+1, r, x);
    }
    int X;
    uint64_t inq(int u, int l, int r) {
        if (!u) return INF;
        int mid = l + r >> 1;
        return std::min(ln[u](X), X <= mid ? inq(ls[u], l, mid) : inq(rs[u], mid+1, r));
    }
    void recycle(int u) {
        if (!u) return;
        bin[++bin[0]] = u;
        recycle(ls[u]), recycle(rs[u]);
    }
    int rt[N];
    inline void update(int id, unsigned k, uint64_t b) {
        upd(rt[id], 1, (int)1e9, Line(k, b));
    }
    inline uint64_t inquire(int id, int x) {
        X=x;
        return inq(rt[id], 1, (int)1e9);
    }
    inline void clear(int id) { recycle(rt[id]), rt[id] = 0; }
} flc;
uint64_t dsum[N];
int n, blksz, blks;
inline void rebuild(int id) {
    flc.clear(id);
    int l = blksz*id + 1, r = std::min(n, blksz * (id+1));
    uint64_t presum = 0;
    for (int i = l; i <= r; i++) {
        presum += d[i];
        flc.update(id, m[i], presum);
    }
    dsum[id] = presum;
}
inline uint64_t inquire(int v) {
    uint64_t ans = INF, presum = 0;
    for (int i = 0; i <= blks; i++) {
        ans = std::min(ans, presum + flc.inquire(i, v));
        presum += dsum[i];
    }
    return ans;
}
int main() {
    int q; in(n), in(q);
    for (int i = 1; i <= n; i++) in(d[i]);
    for (int i = 1; i <= n; i++) in(m[i]);
    blksz = sqrt(n), blks = (n-1) / blksz;
    for (int i = 0; i <= blks; i++)
        rebuild(i);
    for (int op, p, f, v; q--; ) {
        in(op), in(p), in(f), in(v);
        if (op == 1) d[p] = f;
        else m[p] = f;
        rebuild((p-1) / blksz);
        printf("%llu\n", inquire(v));
    }
}