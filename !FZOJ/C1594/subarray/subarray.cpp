#include <cstdio>
#include <cstdint>
#include <algorithm>
constexpr int N = 200004;
constexpr int64_t INF = 1ll << 62;
// #pragma GCC optimize(2, "unroll-loops", "no-stack-protector")
static int a[N];
static int64_t asum[N];
static struct Node {
    struct Line {
        int64_t k = 0, b = INF;
        int64_t operator()(int64_t x) { return k * x + b; }
    } x;
    int lc, rc;
} lctr[N * 4 * 20];
static int tot, n;
class LCTr {
    int rt = 0;
    int64_t P;
    void upd(int& u, int64_t l, int64_t r, Node::Line x) {
        if (!u) u = ++tot;
        int64_t mid = l + r >> 1;
        int64_t xm = x(mid), um = lctr[u].x(mid);
        if (l == r) {
            if (xm < um) std::swap(x, lctr[u].x);
            return;
        }
        int64_t xl = x(l), xr = x(r), ul = lctr[u].x(l), ur = lctr[u].x(r);
        if (xm > um) {
            if (xl < ul) upd(lctr[u].lc, l, mid, x);
            else if (xr < ur) upd(lctr[u].rc, mid+1, r, x);
        } else {
            std::swap(x, lctr[u].x);
            if (ul < xl) upd(lctr[u].lc, l, mid, x);
            else if (ur < xr) upd(lctr[u].rc, mid+1, r, x);
        }
    }
    int64_t inq(int u, int64_t l, int64_t r) {
        if (!u) return INF;
        int64_t mid = l + r >> 1;
        return std::min(lctr[u].x(P),
            P <= mid ? inq(lctr[u].lc, l, mid) : inq(lctr[u].rc, mid+1, r)
        );
    }
public:
    void addline(int64_t k, int64_t b) { upd(rt, -2e12, 2e12, {k, b}); }
    int64_t inquire(int64_t pos) { P=pos; return inq(rt, -2e12, 2e12); }
} pre, suf;
static struct Query {
    int64_t x;
    int pos, id;
    bool operator<(const Query &o) const { return pos < o.pos; }
} q[N];
int64_t ans[N];
int main() {
    int m;
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++)
        scanf("%d", a+i), asum[i] = asum[i-1] + a[i];
    int64_t curx = 0;
    int top = 0;
    for (int i = 1; i <= m; i++) {
        int op, x;
        scanf("%d%d", &op, &x);
        if (op == 1) ++top, q[top] = {curx, x, top}, ans[top] = -(a[x] + curx);
        else curx += x; 
    }
    std::sort(q+1, q+1+top);
    for (int i = 0, j = 1; j <= top; j++) {
        for (; i < q[j].pos; i++)
            pre.addline(i, asum[i]);
        ans[q[j].id] += asum[q[j].pos] + q[j].x * q[j].pos - pre.inquire(q[j].x);
    }
    for (int i = n, j = top; j >= 1; j--) {
        for (; i >= q[j].pos; i--)
            suf.addline(n-i, asum[n]-asum[i]);
        ans[q[j].id] += asum[n] - asum[q[j].pos-1] + q[j].x * (n - q[j].pos + 1) - suf.inquire(q[j].x);
    }
    for (int i = 1; i <= top; i++)
        printf("%lld\n", ans[i]);
    return 0;
}