#include <stdio.h>
constexpr int N = 55, M = 100005;
int n;
int edg[M][2];
using Status = unsigned long long;
struct Node {
    Status to;
    int s;
    inline bool operator[] (int p) { return to >> p & 1; }
    inline Node& operator|= (int p) { to |= 1ull << p; }
} g[N];
namespace SegTr {
Node tr[M << 2];
inline void pushup(int u) {
    tr[u] = tr[u<<1];
    if (tr[u<<1][tr[u<<1|1].s]) tr[u].s |= tr[u<<1|1].s;
}
void build(int u = 1, int l = 1, int r = n) {
    if (l == r) {
        scanf("%d", &tr[u].s);
        tr[u].to = g[tr[u].s];
    }
    int mid = l + r >> 1;
    build(u << 1, l, mid);
    build(u << 1 | 1, mid + 1, r);
    pushup(u);
}
int P, X;
void upd(int u = 1, int l = 1, int r = n) {
    if (l == r) {
        tr[u].s = X;
        tr[u].to = g[X];
        return;
    }
    int mid = l + r >> 1;
    if (P <= mid) upd(u << 1, l, mid);
    else upd(u << 1 | 1, mid + 1, r);
    pushup(u);
}
int L, R;
Node que(int u = 1, int l = 1, int r = n) {
    if (L <= l && r <= R) return tr[u];
    int mid = l + r >> 1;
    if (L <= mid && mid < R) {
        Node left = que(u << 1, l, mid);
        Node right = que(u << 1 | 1, mid + 1, r);
        if (left[right.s]) left.s |= right.s;
        return left;
    }
    if (L <= mid) return que(u << 1, l, mid);
    return que(u << 1 | 1, mid + 1, r);
}
inline void update (int l, int r) { L = l, R = r; upd(); }
inline Node query (int l, int r) { L = l, R = r; return query(); }
}
int main() {
    int T, m, q;
    scanf("%d%d", &T, &m);
    for (int x, y, i = 1; i <= m; i++) {
        scanf("%d%d", &edg[i][0], &edg[i][1]);
    }
    scanf("%d", &n);
    SegTr::build();
    scanf("%d", &q);
    for (int op, x, y, z; q--; ) {
        scanf("%d%d%d", &op, &x, &y);
        if (op == 1) scanf("%d", &z);
        if (op == 1) {
            Node get = SegTr::query(x, y);
            if (get[z])
        }
    }
    return 0;
}