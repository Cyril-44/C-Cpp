#include <bits/stdc++.h>
constexpr int N = 150005;
#define L first
#define R second
using Pll = std::pair<int64_t, int64_t>;
using Pii = std::pair<int, int>;
Pll a[N]; int64_t all[N*2]; std::pair<int64_t, int> allL[N]; Pii b[N], c[N];
inline Pii operator+(Pii x, Pii y) {
    return {std::min(x.L, y.L), std::max(x.R, y.R)};
}
int n, m;
class SegTr1 {
    struct Node {
        int mx, add;
        void pull(int x) { mx += x, add += x; }
    } tr[N << 3];
    int ql, qr, qx;
    void pushdown(int u) { tr[u<<1].pull(tr[u].add), tr[u<<1|1].pull(tr[u].add), tr[u].add = 0; }
    void pushup(int u) { tr[u].mx = std::max(tr[u<<1].mx, tr[u<<1|1].mx); }
    void upd(int u, int l, int r) {
        if (ql <= l && r <= qr) return tr[u].pull(qx);
        int mid = l + r >> 1;
        pushdown(u);
        if (ql <= mid) upd(u<<1, l, mid);
        if (mid < qr) upd(u<<1|1, mid+1, r);
        pushup(u);
    }
    int inq(int u, int l, int r) {
        if (ql <= l && r <= qr) return tr[u].mx;
        int mid = l + r >> 1;
        pushdown(u);
        if (qr <= mid) return inq(u<<1, l, mid);
        if (mid < ql) return inq(u<<1|1, mid+1, r);
        return std::max(inq(u<<1, l, mid), inq(u<<1|1, mid+1, r));
    }
public:
    void update(int l, int r, int x) { ql=l, qr=r, qx=x, upd(1, 1, m); }
    int inquire() { return tr[1].mx; }
} f1;
class SegTr2 { // 记录 -i-E[i], 其中 E[i] 代表 i ~ j 中有多少条边。
    struct Node {
        struct Data {
            int64_t mnsum; int mn, mncnt;
            friend Data operator+(const Data &x, const Data &y) {
                if (x.mn < y.mn) return x;
                else if (y.mn < x.mn) return y;
                else return {x.mnsum + y.mnsum, x.mn, x.mncnt + y.mncnt};
            }
        } dat;
        int add;
        void pull(int x) { dat.mn += x, add += x; }
    } tr[N << 2];
    void pushup(int u) { tr[u].dat = tr[u<<1].dat + tr[u<<1|1].dat; }
    void pushdown(int u) { tr[u<<1].pull(tr[u].add), tr[u<<1|1].pull(tr[u].add), tr[u].add = 0; }
    int ql, qr, qx;
    void upd(int u, int l, int r) {
        if (ql <= l && r <= qr) return tr[u].pull(qx);
        int mid = l + r >> 1;
        pushdown(u);
        if (ql <= mid) upd(u<<1, l, mid);
        if (mid < qr) upd(u<<1|1, mid+1, r);
        pushup(u);
    }
    Node::Data inq(int u, int l, int r) {
        if (ql <= l && r <= qr) return tr[u].dat;
        int mid = l + r >> 1;
        pushdown(u);
        if (qr <= mid) return inq(u<<1, l, mid);
        if (mid < ql) return inq(u<<1|1, mid+1, r);
        return inq(u<<1, l, mid) + inq(u<<1|1, mid+1, r);
    }
public:
    void build(int u=1, int l=1, int r=n) {
        tr[u].add = 0;
        if (l == r) tr[u].dat = {l, -l, 1};
        else {
            int mid = l + r >> 1;
            build(u<<1, l, mid);
            build(u<<1|1, mid+1, r);
            pushup(u);
        }
    }
    void update(int l, int r, int x) {
        // fprintf(stderr, "E[%d...%d] += %d\n", l, r, x);
        ql=l, qr=r, qx=x; upd(1, 1, n);
    }
    int64_t inquire(int lo, int j) {
        ql=lo, qr=j;
        auto res = inq(1, 1, n);
        // fprintf(stderr, "inquire [%d, %d]: mn = %d, (%d+1)*%d - %lld\n", lo, j, res.mn, j, res.mncnt, res.mnsum);
        if (res.mn != -j) return 0;
        return (j+1ll)*res.mncnt - res.mnsum;
    }
} f2;
std::vector<int> result;
class SegTr3 { // 下标为离散化后的 l。每一个 l 存对应的 r。查询 (ql, qr) 的时候，在 result 中返回所有的 l <= qr 且 r >= ql 的 l 所属于的线段下标。
    int tr[N << 2];
    int qp, qx;
    void upd(int u, int l, int r) {
        if (l == r) tr[u] = qx;
        else {
            int mid = l + r >> 1;
            qp <= mid ? upd(u<<1, l, mid) : upd(u<<1|1, mid+1, r);
            tr[u] = std::max(tr[u<<1], tr[u<<1|1]);
        }
    }
    void inq(int u, int l, int r) {
        if (l > qp || tr[u] < qx) return;
        if (l == r) {
            // fprintf(stderr, "Put %d %d, on req %d <= %d && %d >= %d\n", l, tr[u], l, qp, tr[u], qx);
            result.push_back(allL[l].second);
        }
        else {
            int mid = l + r >> 1;
            inq(u<<1, l, mid), inq(u<<1|1, mid+1, r);
        }
    }
public:
    void apply(int lpos, int rval) { qp=lpos, qx=rval; upd(1, 1, n); }
    void inquire(int ql, int qr) /* qr 是 1~n 离散化，ql 只表示大小 */ { qp=qr, qx=ql, result.clear(); inq(1, 1, n); }
} f3;
int main() {
    freopen("evanescia.in", "r", stdin);
    freopen("evanescia.out", "w", stdout);
    int T;
    scanf("%d", &T);
    while (T--) {
        scanf("%d", &n);
        for (int i = 1; i <= n; i++) {
            scanf("%lld%lld", &a[i].L, &a[i].R);
            all[i*2-1] = a[i].L, all[i*2] = a[i].R;
            allL[i] = {a[i].L, i};
        }
        std::sort(all+1, all+1+n*2);
        std::sort(allL+1, allL+1+n);
        auto end = std::unique(all+1, all+1+n*2);
        m = end - all - 1;
        for (int i = 1; i <= n; i++) {
            b[i].L = std::lower_bound(all+1, end, a[i].L) - all;
            b[i].R = std::lower_bound(all+1, end, a[i].R) - all;
            c[allL[i].second].L = i;
            auto tmp = Pii(a[i].R, n);
            c[i].R = std::upper_bound(allL+1, allL+1+n, std::make_pair(a[i].R, n)) - allL-1;
        }

        memset(&f1, 0, sizeof f1);
        f2.build();
        memset(&f3, 0, sizeof f3);
        int64_t ans = 0;
        for (int i = 1, j = 1; j <= n; j++) {
            f1.update(b[j].L, b[j].R, 1);
            for (; f1.inquire() > 2; i++) {
                f1.update(b[i].L, b[i].R, -1);
                f3.apply(c[i].L, 0);
            }
            f3.inquire(b[j].L, c[j].R);
            for (int p : result)
                f2.update(i, p, -1);
            f3.apply(c[j].L, b[j].R);
            ans += f2.inquire(i, j);
        }
        // fprintf(stderr, "==========\n");
        printf("%lld\n", ans);
    }
}