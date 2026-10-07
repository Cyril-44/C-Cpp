#include <cstdio>
#ifdef CLANGD
constexpr int N = 2;
#else
constexpr int N = 200005;
#endif
int n;
class SegTr {
    int top;
    struct Node {
        int sum, ls, rs;
    } tr[N * 20];
    int P, X, L, R;
    void upd(int& u, int l, int r) {
        tr[++top] = tr[u]; u = top;
        if (l == r) { tr[u].sum += X; return; }
        int mid = l + r >> 1;
        if (P <= mid) upd(tr[u].ls, l, mid);
        else upd(tr[u].rs, mid+1, r);
        tr[u].sum = tr[tr[u].ls].sum + tr[tr[u].rs].sum;
    }
    int inq(int u, int l, int r) {
        if (!u) return 0;
        if (L <= l && r <= R) return tr[u].sum;
        int mid = l + r >> 1, res = 0;
        if (L <= mid) res += inq(tr[u].ls, l, mid);
        if (mid < R) res += inq(tr[u].rs, mid+1, r);
        return res;
    }
    int rt[N];
public:
    void update(int u, int p) { P=p, X=1; upd(rt[u] = rt[u-1], 0, n-1); }
    int inquire(int u, int l, int r) { L=l, R=r; return inq(rt[u], 0, n-1); }
} f;
int main() {
    freopen("seg.in", "r", stdin);
    freopen("seg.out", "w", stdout);
    int q; scanf("%d%d", &n, &q);
    for (int i = 1, ai; i <= n; i++) {
        scanf("%d", &ai);
        f.update(i, ai);
    }
    for (int l, r; q--;) {
        scanf("%d%d", &l, &r);
        int d = 1, u = n;
        while (d <= u) {
            int mid = d + u >> 1;
            if (f.inquire(r, 0, mid-1) - f.inquire(l-1, 0, mid-1) == mid) d = mid + 1;
            else u = mid - 1;
        }
        printf("%d\n", u);
    }
    return 0;
}