// Window Calibration, independent partial solution for groups 1, 2 and 3.
// Segment tree on the ORIGINAL array. A tag is
//   a[i] <- mul*a[i] + c0 + c1*i + c2*binom(i,2).
// k=1 calibration is quadratic assignment. General k is simulated in O(length).
// Exact 2*n-1 packed preorder layout, 20 bytes per node.
// Compile: g++ -std=c++17 -O2 partial_special.cpp -o partial_special
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <vector>

using u32 = uint32_t;
using u64 = uint64_t;
constexpr u32 P = 998244353;
constexpr u32 INV6 = 166374059;
inline u32 add(u32 a, u32 b) { u32 c = a + b; return c >= P ? c - P : c; }
inline u32 sub(u32 a, u32 b) { return a >= b ? a - b : a + P - b; }
inline u32 mul(u32 a, u32 b) { return u64(a) * b % P; }
inline u32 c2(u32 n) { return n < 2 ? 0 : u64(n) * (n - 1) / 2 % P; }
inline u32 c3(u32 n) { return n < 3 ? 0 : mul(mul(mul(n, n - 1), n - 2), INV6); }

struct FastIO {
    static constexpr int B = 1 << 20;
    char in[B], out[B];
    int ip = 0, il = 0, op = 0;
    int get() {
        if (ip == il) { il = int(fread(in, 1, B, stdin)); ip = 0; if (!il) return EOF; }
        return in[ip++];
    }
    u32 read() {
        int c;
        do { c = get(); } while (c <= 32 && c != EOF);
        if (c == EOF) std::exit(2);
        u32 x = 0;
        do { x = x * 10 + c - '0'; c = get(); } while (c > 32 && c != EOF);
        return x;
    }
    void write(u32 x) {
        char s[16]; int k = 0;
        do { s[k++] = char('0' + x % 10); x /= 10; } while (x);
        if (op + 20 > B) flush();
        while (k) out[op++] = s[--k];
        out[op++] = '\n';
    }
    void flush() { fwrite(out, 1, op, stdout); op = 0; }
    ~FastIO() { flush(); }
};

struct SegmentTree {
    struct Node { u32 sum = 0, factor = 1, c0 = 0, c1 = 0, c2 = 0; };
    int n;
    std::vector<Node> t;

    explicit SegmentTree(const std::vector<u32>& a) : n(int(a.size()) - 1), t(size_t(n) * 2 - 1) {
        build(0, 1, n, a);
    }
    static int right(int p, int l, int mid) { return p + 2 * (mid - l + 1); }
    void build(int p, int l, int r, const std::vector<u32>& a) {
        if (l == r) { t[p].sum = a[l]; return; }
        int mid = (l + r) / 2, q = right(p, l, mid);
        build(p + 1, l, mid, a); build(q, mid + 1, r, a);
        t[p].sum = add(t[p + 1].sum, t[q].sum);
    }
    void apply(int p, int l, int r, u32 factor, u32 q0, u32 q1, u32 q2) {
        Node& x = t[p];
        x.sum = mul(x.sum, factor);
        if (q0) x.sum = add(x.sum, mul(q0, r - l + 1));
        if (q1) x.sum = add(x.sum, mul(q1, sub(c2(r + 1), c2(l))));
        if (q2) x.sum = add(x.sum, mul(q2, sub(c3(r + 1), c3(l))));
        x.factor = mul(x.factor, factor);
        x.c0 = add(mul(x.c0, factor), q0);
        x.c1 = add(mul(x.c1, factor), q1);
        x.c2 = add(mul(x.c2, factor), q2);
    }
    void push(int p, int l, int r) {
        const Node x = t[p];
        if (l == r || (x.factor == 1 && !x.c0 && !x.c1 && !x.c2)) return;
        int mid = (l + r) / 2;
        apply(p + 1, l, mid, x.factor, x.c0, x.c1, x.c2);
        apply(right(p, l, mid), mid + 1, r, x.factor, x.c0, x.c1, x.c2);
        t[p].factor = 1; t[p].c0 = t[p].c1 = t[p].c2 = 0;
    }
    void update(int p, int l, int r, int ql, int qr, u32 factor, u32 q0, u32 q1, u32 q2) {
        if (ql <= l && r <= qr) { apply(p, l, r, factor, q0, q1, q2); return; }
        push(p, l, r);
        int mid = (l + r) / 2, q = right(p, l, mid);
        if (ql <= mid) update(p + 1, l, mid, ql, qr, factor, q0, q1, q2);
        if (qr > mid) update(q, mid + 1, r, ql, qr, factor, q0, q1, q2);
        t[p].sum = add(t[p + 1].sum, t[q].sum);
    }
    u32 query(int p, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return t[p].sum;
        push(p, l, r);
        int mid = (l + r) / 2; u32 ans = 0;
        if (ql <= mid) ans = query(p + 1, l, mid, ql, qr);
        if (qr > mid) ans = add(ans, query(right(p, l, mid), mid + 1, r, ql, qr));
        return ans;
    }
    void collect(int p, int l, int r, int ql, int qr, int base, std::vector<u32>& a) {
        if (l == r) { a[l - base] = t[p].sum; return; }
        push(p, l, r);
        int mid = (l + r) / 2;
        if (ql <= mid) collect(p + 1, l, mid, ql, qr, base, a);
        if (qr > mid) collect(right(p, l, mid), mid + 1, r, ql, qr, base, a);
    }
    void overwrite(int p, int l, int r, int ql, int qr, int base, const std::vector<u32>& a) {
        if (l == r) { t[p] = {a[l - base], 1, 0, 0, 0}; return; }
        push(p, l, r);
        int mid = (l + r) / 2, q = right(p, l, mid);
        if (ql <= mid) overwrite(p + 1, l, mid, ql, qr, base, a);
        if (qr > mid) overwrite(q, mid + 1, r, ql, qr, base, a);
        t[p].sum = add(t[p + 1].sum, t[q].sum);
    }
    u32 point(int i) { return query(0, 1, n, i, i); }
    void calibrate(int l, int r, int k) {
        if (k == r - l - 1) return;
        if (k == 1) {
            u32 A = point(l), B = sub(point(l + 1), A);
            u32 C = sub(add(point(l + 2), A), mul(point(l + 1), 2));
            u32 p0 = add(sub(A, mul(B, l)), mul(C, c2(l + 1)));
            u32 p1 = sub(B, mul(C, l));
            update(0, 1, n, l, r, 0, p0, p1, C);
            return;
        }
        std::vector<u32> a(r - l + 1);
        collect(0, 1, n, l, r, l, a);
        u32 w0 = 0;
        for (int i = 0; i < k; ++i) w0 = add(w0, a[i]);
        u32 w1 = add(sub(w0, a[0]), a[k]);
        u32 w2 = add(sub(w1, a[1]), a[k + 1]);
        u32 C = sub(add(w2, w0), mul(w1, 2));
        u32 delta = add(sub(w2, w1), C);
        for (int j = k + 2; j < int(a.size()); ++j) {
            a[j] = add(a[j - k], delta);
            delta = add(delta, C);
        }
        overwrite(0, 1, n, l + k + 2, r, l, a);
    }
};

int main() {
    static FastIO io;
    int n = int(io.read());
    std::vector<u32> a(n + 1);
    for (int i = 1; i <= n; ++i) a[i] = io.read();
    SegmentTree tree(a);
    std::vector<u32>().swap(a);
    int m = int(io.read());
    while (m--) {
        int op = int(io.read()), l = int(io.read()), r = int(io.read());
        if (op == 1) tree.calibrate(l, r, int(io.read()));
        else if (op == 2) {
            u32 u = io.read(), v = io.read();
            tree.update(0, 1, n, l, r, u, v, 0, 0);
        } else io.write(tree.query(0, 1, n, l, r));
    }
}
