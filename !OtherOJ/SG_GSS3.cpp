#include <stdio.h>
#include <vector>
#include <tuple>
constexpr int N = 50005;
int a[N];
struct SegTree {
    inline int ask(int l, int r) { L = l, R = r; return std::get<1>(query(1, 1, n)); }
    inline void modify(int p, int x) { P = p, X = x; upd(1, 1, n); }
    SegTree(int n) : n(n), tr(n + 1 << 2) { build(1, 1, n); }
private:
    int n;
    struct Node { int lmx, rmx, mx, sum; };
    inline void pushup(int u) {
        tr[u].sum = tr[u<<1].sum + tr[u<<1|1].sum;
        tr[u].mx = std::max(std::max(tr[u<<1].mx, tr[u<<1|1].mx), tr[u<<1].rmx + tr[u<<1|1].lmx);
        tr[u].lmx = std::max(tr[u<<1].lmx, tr[u<<1].sum + tr[u<<1|1].lmx);
        tr[u].rmx = std::max(tr[u<<1|1].rmx, tr[u<<1|1].sum + tr[u<<1].rmx);
    }
    void build(int u, int l, int r) {
        if (l == r) tr[u] = {a[l], a[l], a[l], a[l]};
        else {
            int mid = l + r >> 1;
            build(u << 1, l, mid);
            build(u << 1 | 1, mid + 1, r);
            pushup(u);
        }
    }
    int P, X;
    void upd(int u, int l, int r) {
        if (l == r) tr[u] = {X, X, X, X};
        else {
            int mid = l + r >> 1;
            if (P <= mid) upd(u << 1, l, mid);
            else upd(u << 1 | 1, mid + 1, r);
            pushup(u);
        }
    }
    int L, R;
    std::tuple<int,int,int, int> query(int u, int l, int r) {
        if (L <= l && r <= R) return {tr[u].lmx, tr[u].mx, tr[u].rmx, tr[u].sum};
        int mid = l + r >> 1;
        if (L <= mid && mid < R) {
            auto [ll, lm, lr, ls] = query(u << 1, l, mid);
            auto [rl, rm, rr, rs] = query(u << 1 | 1, mid + 1, r);
            return {std::max(ll, ls + rl), std::max(std::max(lm, rm), lr + rl), std::max(rr, rs + lr), ls + rs};
        }
        if (L <= mid) return query(u << 1, l, mid);
        return query(u << 1 | 1, mid + 1, r);
    }
    std::vector<Node> tr;
};
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    SegTree f(n);
    int q;
    scanf("%d", &q);
    for (int i = 1, opt, x, y; i <= q; i++) {
        scanf("%d%d%d", &opt, &x, &y);
        if (opt) printf("%d\n", f.ask(x, y));
        else f.modify(x, y);
    }
}