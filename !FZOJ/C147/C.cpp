#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <vector>
constexpr int N = 200005;
constexpr long long INF = 0x7f7f7f7f7f7f7f7fll;
struct Node {
    int x, w, v1, v2;
    inline bool operator< (const Node &rhs) const {
        return x < rhs.x;
    }
} a[N];
int n;
#define umin(x, y) ((x) = std::min((x), (y)))
#define umax(x, y) ((x) = std::max((x), (y)))
using LL = long long;
namespace SegTree {
    LL tr[N << 2], lazy[N << 2];
    inline void build() {
        memset(tr, 0x7f, sizeof tr);
        memset(lazy, 0x7f, sizeof lazy);
    }
    inline void pushdown(int u) {
        if (lazy[u] ^ INF) {
            umin(tr[u<<1], lazy[u]);
            umin(tr[u<<1|1], lazy[u]);
            umin(lazy[u<<1], lazy[u]);
            umin(lazy[u<<1|1], lazy[u]);
            lazy[u] = INF;
        }
    }
    void upd(int L, int R, int X, int u=1, int l=1, int r=n) {
        if (L <= l && r <= R) {
            umin(tr[u], (LL)X);
            umin(lazy[u], (LL)X);
            return;
        }
        int mid = l + r >> 1;
        pushdown(u);
        if (L <= mid) upd(L, R, X, u<<1, l, mid);
        if (mid < R) upd(L, R, X, u<<1|1, mid+1, r);
        tr[u] = std::max(tr[u << 1], tr[u << 1 | 1]);
    }
    inline LL query(int p, int u=1, int l=1, int r=n) {
        if (l == r) return tr[u];
        int mid = l + r >> 1;
        pushdown(u);
        if (p <= mid) return query(p, u<<1, l, mid);
        else return query(p, u<<1|1, mid+1, r);
    }
    inline int bsearch(int p, int u=1, int l=1, int r=n) {
        if (l == r) return tr[u] <= p;
        if (tr[u] <= p) return r - l + 1;
        int mid = l + r >> 1;
        pushdown(u);
        int left = bsearch(p, u<<1, l, mid);
        if (left == mid - l + 1) return left + bsearch(p, u<<1|1, mid+1, r);
        return left;
    }
}
namespace BruteForce {
    int f[N];
    inline void solve(int n) {
        memset(f, 0x7f, sizeof(int) * (n+2));
        f[0] = -f[0];
        for (int i = 1; i <= n; i++) {
            int pre = a[i].x - a[i].w, nxt = a[i].x + a[i].w;
            for (int j = i; j >= 0; j--)
                if (f[j] <= pre)
                    f[j+1] = std::min(f[j+1], nxt);
            for (int i = 1; i <= n; i++)
                fprintf(stderr, "%d ", f[i]);
            fprintf(stderr, "\n");
        }
        int ans = 0;
        for (int i = 1; i <= n; i++)
            if (f[i] ^ 0x7f7f7f7f) ans = i;
        printf("%d\n", ans);
    }
}
int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%d%d", &a[i].x, &a[i].w);
    }
    std::sort(a+1, a+1 + n);
    // if (n <= 3000) return BruteForce::solve(n), 0;
    SegTree::build();
    for (int i = 1; i <= n; i++) {
        int f = SegTree::bsearch(a[i].x - a[i].w);
        SegTree::upd(1, f+1, a[i].x + a[i].w);
        // fprintf(stderr, "%d %d\n", f, a[i].x - a[i].w);
        // for (int i = 1; i <= n; i++)
        //     printf("%d ", SegTree::query(i));
        // putchar('\n');
    }
    int ans = 0;
    for (int i = 1; i <= n; i++) {
        if (SegTree::query(i) != INF) ans = i;
    }
    printf("%d\n", ans);
    return 0;
}
