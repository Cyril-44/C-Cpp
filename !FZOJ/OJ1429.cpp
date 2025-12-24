#include <stdio.h>
#include <string.h>
#include <numeric>
#include <algorithm>
#include <functional>
constexpr int N = 100005, K = 3;
#define umin(x, y) ((x) < (y) ? (x) : (x) = (y))
#define umax(x, y) ((x) < (y) ? (x) = (y) : (x))
struct Node {
    int v[K]; // 当前位置，最早位置，最晚位置。
    int l, r, mx, dat;
    std::pair<int,int> rg[K];
} tr[N];
int a[N];
inline void pushup(Node &u) {
    u.mx = u.dat;
    if (u.l) umax(u.mx, tr[u.l].mx);
    if (u.r) umax(u.mx, tr[u.r].mx);
    for (int k = 0; k ^ K; k++) {
        u.rg[k] = {u.v[k], u.v[k]};
        if (u.l) umin(u.rg[k].first, tr[u.l].rg[k].first), umax(u.rg[k].second, tr[u.l].rg[k].second);
        if (u.r) umin(u.rg[k].first, tr[u.r].rg[k].first), umax(u.rg[k].second, tr[u.r].rg[k].second);
    }
}
int build(int l = 1, int r = a[0], int k = 0) {
    int mid = l + r >> 1;
    std::nth_element(a+l, a+mid, a+r + 1, [k](int x, int y){ return tr[x].v[k] < tr[y].v[k]; });
    int u = a[mid];
    if (l ^ mid) tr[u].l = build(l, mid-1, (k+1) % K);
    if (mid ^ r) tr[u].r = build(mid+1, r, (k+1) % K);
    pushup(tr[u]);
    return u;
}
int L, R;
int root;
int ans;
void que(int u = root) { // v0 \in [L, R], v1 \notin [L, R], v2 \notin [L, R]
    if (!u || tr[u].mx <= ans ||
        tr[u].rg[0].second < L || tr[u].rg[1].first > R || 
        L <= tr[u].rg[1].first || tr[u].rg[2].second <= R)
        return;
    if (L <= tr[u].rg[0].first && tr[u].rg[0].second <= R && tr[u].rg[1].second < L && tr[u].rg[2].first > R)
        return umax(ans, tr[u].mx), void();
    if (L <= tr[u].v[0] && tr[u].v[0] <= R && tr[u].v[1] < L && tr[u].v[2] > R) umax(ans, tr[u].dat);
    que(tr[u].l), que(tr[u].r);
}
int f[N], g[N];
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &tr[i].dat);
        tr[i].v[0] = i;
        tr[i].v[1] = f[tr[i].dat];
        f[tr[i].dat] = i;
    }
    memset(g, 0x3f, sizeof g);
    for (int i = n; i >= 1; i--) {
        tr[i].v[2] = g[tr[i].dat];
        g[tr[i].dat] = i;
    }
    std::iota(a+1, a+1 + (a[0] = n), 1);
    root = build();
    int lastans = 0;
    for (int l, r; m--; ) {
        scanf("%d%d", &l, &r);
        l = (l + lastans) % n + 1;
        r = (r + lastans) % n + 1;
        if (l > r) std::swap(l, r);
        L = l, R = r, ans = 0; que();
        printf("%d\n", lastans = ans);
    }
    return 0;
}