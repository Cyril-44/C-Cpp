#include <stdio.h>
#include <string.h>
#include <stdlib.h>
constexpr int N = 100005;
int M, n;
namespace SegTr {
    struct Node {
        int sum, size;
        int add, mul;
        inline void pull(int tadd, int tmul) {
            sum = (1ll * sum * tmul + 1ll * tadd * size) % M;
            add = (1ll * add * tmul + tadd) % M;
            mul = 1ll * mul * tmul % M;
        }
    } tr[N << 2];
    inline void pushup(int u) {
        tr[u].sum = (tr[u<<1].sum + tr[u<<1|1].sum) % M;
    }
    inline void pushdown(int u) {
        if (tr[u].add || tr[u].mul!=1) {
            tr[u<<1].pull(tr[u].add, tr[u].mul);
            tr[u<<1|1].pull(tr[u].add, tr[u].mul);
            tr[u].add=0, tr[u].mul=1;
        }
    }
    void build(int u=1, int l=1, int r=n) {
        tr[u].mul = 1;
        tr[u].size = r - l + 1;
        if (l == r) {
            scanf("%d", &tr[u].sum);
            tr[u].sum %= M;
            return;
        }
        int mid = l + r >> 1;
        build(u << 1, l, mid);
        build(u << 1 | 1, mid+1, r);
        pushup(u);
    }
    int L, R, X, Y;
    int que(int u, int l, int r) {
        if (R < l || r < L) return 0;
        if (L <= l && r <= R) return tr[u].sum;
        int mid = l + r >> 1;
        pushdown(u);
        return (que(u<<1, l, mid) + que(u<<1|1, mid+1, r)) % M;
    }
    void upd(int u, int l, int r) {
        if (R < l || r < L) return;
        if (L <= l && r <= R) {
            tr[u].pull(X, Y);
            return;
        }
        int mid = l + r >> 1;
        pushdown(u);
        upd(u<<1, l, mid);
        upd(u<<1|1, mid+1, r);
        pushup(u);
    }
    inline int query(int l, int r) {
        L = l, R = r;
        return que(1, 1, n);
    }
    inline void updadd(int l, int r, int x) {
        L = l, R = r, X = x, Y = 1;
        upd(1, 1, n);
    }
    inline void updmul(int l, int r, int x) {
        L = l, R = r, X = 0, Y = x;
        upd(1, 1, n);
    }
}
int main() {
    int q;
    scanf("%d%d%d", &n, &q, &M);
    SegTr::build();
    for (int op, l, r, k; q--; ) {
        scanf("%d%d%d", &op, &l, &r);
        if (op != 3) scanf("%d", &k);
        switch (op) {
        case 1: SegTr::updmul(l, r, k); break;
        case 2: SegTr::updadd(l, r, k); break;
        case 3: printf("%d\n", SegTr::query(l, r));
        }
#ifdef DEBUG
        for (int i = 1; i <= n; i++)
            fprintf(stderr, "%d ", SegTr::query(i, i));
        fprintf(stderr, "\n");
#endif
    }
#ifdef DEBUG
    system("grep VmPeak /proc/$PPID/status");
#endif
    return 0;
}
