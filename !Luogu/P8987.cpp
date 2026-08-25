#include <cstdio>
#include <cstdint>
#include <algorithm>
#include <vector>
#include <numeric>
#include <tuple>
#ifdef CLANGD
constexpr int N = 204;
#else
constexpr int N = 200004;
#endif
constexpr int64_t INF = 1ll << 62;
int64_t a[N], v[N];
int b[N];
std::vector<int> swt[N];
std::vector<std::tuple<int,int,int>> qry[N];
int idxs[N], _idxs[N];
int q[N];
int n;
struct ConvexHull { // Up Convex Hull, Maintaining min
    struct Line {
        int64_t k, b;
        int64_t operator()(int64_t x) const { return k * x + b; }
        int64_t intersect(const Line &l) const { return (l.b - b) / (k - l.k); }
    } l[N];
    int64_t cuts[N];
    int top, cur;
    void add(const Line &nl) { // Requires k decreasing
        while (top && nl.k == l[top].k && nl.b < l[top].b) --top;
        if (top && nl.k == l[top].k) return;
        while (top && nl.intersect(l[top]) <= cuts[top]) --top;
        l[++top] = nl;
        if (top == 1) cuts[top] = 0;
        else cuts[top] = nl.intersect(l[top-1]);
    }
    int64_t inquire(int pos) { // Requires pos increasing
        while (cur < top && cuts[cur+1] < pos) ++cur;
        return l[cur](pos);
    }
    void init() { top = 0, cur = 1; }
} minch;
void search(int l, int r, int il, int ir) {
    if (l == r) {
        for (int i = il; i <= ir; i++)
            swt[l].push_back(i);
        return;
    }
    int mid = l + r >> 1;
    minch.init();
    for (int j = l; j <= mid; j++)
        minch.add({-b[j], v[j]});
    int lidx = il, ridx = ir;
    for (int i = il; i <= ir; i++)
        if (a[i] >= minch.inquire(i))
            _idxs[lidx++] = i;
        else
            _idxs[ridx--] = i;
    std::reverse(_idxs+lidx, _idxs+ir+1);
    search(l, mid, il, ridx);
    search(mid+1, r, lidx, ir);
}
class BIT {
    int64_t tr[N];
public:
    void upd(int p, int64_t x) { for (; p <= n; p += p & -p) tr[p] += x; }
    int64_t sum(int p) { int64_t res = 0; for (; p > 0; p -= p & -p) res += tr[p]; return res; }
    int64_t sum(int l, int r) { return sum(r) - sum(l-1); }
} fs1base, fs1add;
class SegTr {
    struct Node {
        int64_t sum = 0, val = 0, min = INF, max = -INF, cov = -1;
        int cnt = 0; // 区间有多少个有效空位
        int minpos, maxpos; // 事实上 minmax 就是最前面和最后面的有效位置
        int add = 0; // 区间 +i 加了几次
        void pull(int64_t c, int a) {
            if (c != -1) {
                cov = c, add = 0;
                sum = 1ll * c * cnt;
                min = max = c;
            }
            if (a) {
                add += a;
                sum += a * val;
                min += 1ll * a * minpos, max += 1ll * a * maxpos;
            }
        }
    } tr[N << 2];
    void pushup(int u) {
        tr[u].sum = tr[u<<1].sum + tr[u<<1|1].sum;
        tr[u].val = tr[u<<1].val + tr[u<<1|1].val;
        if (tr[u<<1].min <= tr[u<<1|1].min) tr[u].min = tr[u<<1].min, tr[u].minpos = tr[u<<1].minpos;
        else tr[u].min = tr[u<<1|1].min, tr[u].minpos = tr[u<<1|1].minpos;
        if (tr[u<<1|1].max >= tr[u<<1].max) tr[u].max = tr[u<<1|1].max, tr[u].maxpos = tr[u<<1|1].maxpos;
        else tr[u].max = tr[u<<1].max, tr[u].maxpos = tr[u<<1].maxpos;
        tr[u].cnt = tr[u<<1].cnt + tr[u<<1|1].cnt;
    }
    void pushdown(int u) {
        tr[u<<1].pull(tr[u].cov, tr[u].add), tr[u<<1|1].pull(tr[u].cov, tr[u].add);
        tr[u].cov = -1, tr[u].add = 0;
    }
    int L, R, P; int64_t X;
    void upd1(int u, int l, int r) { // 将 a_P 放进线段树
        if (l == r) { tr[u] = {.sum=X, .val=l, .min=X, .max=X, .cov=-1, .cnt=1, .minpos=l, .maxpos=l, .add=0}; return; }
        int mid = l + r >> 1; pushdown(u);
        P <= mid ? upd1(u<<1, l, mid) : upd1(u<<1|1, mid+1, r);
        pushup(u);
    }
    void upd2(int u, int l, int r) {
        if (tr[u].max <= X) return;
        if (tr[u].min > X) return tr[u].pull(X, 0);
        int mid = l + r >> 1; pushdown(u);
        upd2(u<<1, l, mid); upd2(u<<1|2, mid+1, r);
        pushup(u);
    }
    int64_t inq(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u].sum;
        int mid = l + r >> 1; int64_t res = 0; pushdown(u);
        if (L <= mid) res += inq(u<<1, l, mid);
        if (mid < R) res += inq(u<<1|1, mid+1, r);
        return res;
    }
public:
    void build(int u=1, int l=1, int r=n) {
        if (l == r) { tr[u].minpos = tr[u].maxpos = l; return; }
        int mid = l + r >> 1;
        build(u<<1, l, mid), build(u<<1|1, mid+1, r);
    }
    void put(int pos, int64_t x) { P=pos, X=x; upd1(1, 1, n); }
    void update(int cnt) { tr[1].pull(-1, cnt); }
    int64_t inquire(int l, int r) { L=l, R=r; return inq(1, 1, n); }
} fs2;
int main() {
    int q;
    scanf("%d%d", &n, &q);
    for (int i = 1; i <= n; i++)
        scanf("%lld", &a[i]);
    int tm = 0, curb = 0;
    while (q--) {
        int op; scanf("%d", &op);
        switch (op) {
        case 1: scanf("%lld", &v[tm++]); break;
        case 2: ++b[tm], ++curb; break;
        case 3: {
            int x, y; scanf("%d%d", &x, &y);
            qry[tm].emplace_back(x, y, curb);
        } break;
        }
    }
    std::iota(idxs+1, idxs+1+n, 1);
    search(1, n, 1, n);
    for (int i = 1; i <= n; i++)
        fs1base.upd(i, a[i]), fs1add.upd(i, i);
    curb = 0;
    for (int i = 0; i < tm; i++) {
        for (const auto &[l, r, qb] : qry[i]) {
            if (curb < qb) fs2.update(qb - curb), curb = qb;
            printf("%lld\n", fs1base.sum(l, r) + fs1add.sum(l, r) * qb + fs2.inquire(l, r));
        }
        if (curb < b[i]) fs2.update(b[i] - curb), curb = b[i];
        for (int x : swt[i]) {
            fs1base.upd(x, -a[x]);
            fs1add.upd(x, -x);
            fs2.put(x, v[i]);
        }
    }
    return 0;
}
