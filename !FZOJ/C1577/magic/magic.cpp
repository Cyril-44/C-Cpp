#include <cstdio>
#include <algorithm>
#include <cstring>
constexpr int N = 200005;
constexpr int64_t INF = 1ll << 54;
int n, a[N];
class KTT {
    struct Line {
        int64_t b; int k;
        Line& operator+=(const Line& l) { b+=l.b, k+=l.k; return *this; }
        friend Line operator+(Line a, const Line& b) { return a += b; }
        void operator+=(int64_t val) { b += k * val; }
        friend Line operator^(const Line &p, const Line &q) {
            // if (q.k == -1) return p; if (p.k == -1) return q; 
            if (p.b < q.b) return p; if (q.b < p.b) return q;
            if (p.k < q.k) return p; return q;
        }
        friend int64_t operator&(const Line &p, const Line &q) {
            int64_t db = p.b - q.b; int dk = q.k - p.k;
            if (!db || !dk || ((db>0) ^ (dk>0))) return INF;
            return db / dk;
        }
    };
#define intersects(l1, l2, l3) l1&l2, l1&l3, l2&l3
    struct Node {
        Line f[2][2], sum;
        int64_t cert, add = 0;
        Line* operator[](bool x) { return f[x]; }
        const Line* operator[](bool x) const { return f[x]; }
        int64_t operator()() const { return std::min({f[0][1].b, f[1][0].b, f[1][1].b}); }
        friend Node operator+(const Node& ls, const Node& rs) { return {
          {{ls[0][0] + rs[1][0] ^ ls[0][1] + rs[0][0] ^ ls[0][1] + rs[1][0],
            ls[0][0] + rs[1][1] ^ ls[0][1] + rs[0][1] ^ ls[0][1] + rs[1][1]},
           {ls[1][0] + rs[1][0] ^ ls[1][1] + rs[0][0] ^ ls[1][1] + rs[1][0],
            ls[1][0] + rs[1][1] ^ ls[1][1] + rs[0][1] ^ ls[1][1] + rs[1][1]}},
            ls.sum + rs.sum,
            std::min({ ls.cert, rs.cert,
                intersects(ls[0][0] + rs[1][0], ls[0][1] + rs[0][0], ls[0][1] + rs[1][0]),
                intersects(ls[0][0] + rs[1][1], ls[0][1] + rs[0][1], ls[0][1] + rs[1][1]),
                intersects(ls[1][0] + rs[1][0], ls[1][1] + rs[0][0], ls[1][1] + rs[1][0]),
                intersects(ls[1][0] + rs[1][1], ls[1][1] + rs[0][1], ls[1][1] + rs[1][1])
            })
        }; }
#define foreach for (int i = 0; i <= 1; i++) for (int j = 0; j <= 1; j++) 
        void set(int64_t val) { f[0][0] = {0, 0}; f[0][1] = f[1][0] = {INF, 1}; f[1][1] = sum = {val, 1}; cert=INF; }
        void apply(int64_t v) { foreach f[i][j]+=v; sum+=v; add+=v; cert-=v; }
#undef foreach
    } tr[N << 2];
    int L, R;
    void apply(int u, int l, int r, int64_t add) {
        if (tr[u].cert >= add) return tr[u].apply(add);
        int mid = l + r >> 1;
        apply(u<<1, l, mid, add + tr[u].add);
        apply(u<<1|1, mid+1, r, add + tr[u].add);
        tr[u] = tr[u<<1] + tr[u<<1|1];
    }
    void pushdown(int u, int l, int r) {
        if (tr[u].add) {
            int mid = l + r >> 1;
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
public:
    void build(int u = 1, int l = 1, int r = n) {
        if (l == r) return tr[u].set(a[l]);
        int mid = l + r >> 1;
        build(u<<1, l, mid), build(u<<1|1, mid+1, r);
        tr[u] = tr[u<<1] + tr[u<<1|1];
    }
    void update(int l, int r, int v) { L=l, R=r; upd(1, 1, n, v); }
    int64_t inquireF(int l, int r) { L=l, R=r; return inq(1, 1, n)(); }
    int64_t inquireS(int l, int r) { L=l, R=r; return inq(1, 1, n).sum.b; }
} fs;
int main() {
    int q;
    scanf("%d%d", &n, &q);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    fs.build();
    for (int op, l, r, v; q--; ) {
        scanf("%d%d%d", &op, &l, &r);
        if (op == 1) {
            scanf("%d", &v);
            fs.update(l, r, v);
        } else {
            int64_t ans = fs.inquireS(l, r);
            if (r - l) ans = std::min(ans, 2*fs.inquireF(l, r));
            printf("%lld.%c00\n", ans>>1, ans&1?'5':'0');
        }
    }
    return 0;
}