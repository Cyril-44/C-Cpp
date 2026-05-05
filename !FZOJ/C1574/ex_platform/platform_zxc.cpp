#include <bits/stdc++.h>
using namespace std;
#define fr(i, a, b) for (int i = (a); i <= (b); ++i)
#define rp(i, a, b) for (int i = (a); i >= (b); --i)
const int N = 1e5 + 5;
const long long inf = 1e18;
int n, fa[N], sz[N], m, to[N];
long long ans, mn[N], b[N << 1];
vector<pair<int, int>> h;
inline int find(int x) {
    return x == fa[x] ? x : (fa[x] = find(fa[x]));
}
struct node {
    int l, r, id;
} a[N], c[N];
inline bool cmp1(node l, node r) {
    return l.l != r.l ? l.l < r.l : l.r > r.r;
}
inline bool cmp2(node l, node r) {
    return l.r != r.r ? l.r < r.r : l.l > r.l;
}
struct node2 {
    long long mx1, mx2;
    int id1, id2;
    inline void upd(long long mx, int id) {
        if (mx > mx1) {
            if (id != id1) mx2 = mx1, id2 = id1;
            mx1 = mx;
            id1 = id;
        } else if (mx > mx2 && id != id1)
            mx2 = mx, id2 = id;
        return;
    }
    inline node2 operator+(const node2 &u) const {
        node2 v = u;
        if (id1) v.upd(mx1, id1);
        if (id2) v.upd(mx2, id2);
        return v;
    }
} t[N << 1];
inline void upd(int x, int mx, int id) {
    for (; x <= m; x += (x & -x)) t[x].upd(mx, id);
}
inline node2 query(int x) {
    node2 rs = {-inf, -inf, 0, 0};
    for (; x; x -= (x & -x)) rs = rs + t[x];
    return rs;
}
int main() {
    scanf("%d", &n);
    fr(i, 1, n) {
        fa[i] = i;
        a[i].id = i;
        sz[i] = 1;
        scanf("%d%d", &a[i].l, &a[i].r);
        b[++m] = a[i].l, b[++m] = a[i].r;
    }
    sort(b + 1, b + 1 + m);
    m = unique(b + 1, b + 1 + m) - b - 1;
    fr(i, 1, n) a[i].l = lower_bound(b + 1, b + 1 + m, a[i].l) - b, a[i].r = lower_bound(b + 1, b + 1 + m, a[i].r) - b,
                c[i] = a[i];
    sort(a + 1, a + 1 + n, cmp1);
    sort(c + 1, c + 1 + n, cmp2);
    while (sz[find(1)] != n) {
        fr(i, 1, n) fa[i] = find(i), mn[i] = inf, assert(fa[i]);
        fr(i, 1, m) t[i] = {-inf, -inf, 0, 0};
        fr(i, 1, n) {
            int id = fa[a[i].id];
            node2 x = query(m - a[i].l);
            long long z;
            int id1;
            if (x.id1 == id)
                z = x.mx2, id1 = x.id2;
            else
                z = x.mx1, id1 = x.id1;
            z = -z - b[a[i].l];
            if (z < mn[id]) mn[id] = z, to[id] = id1;
            upd(m + 1 - a[i].r, -b[a[i].r], id);
        }
        fr(i, 1, m) t[i] = {-inf, -inf, 0, 0};
        fr(i, 1, n) {
            int id = fa[c[i].id];
            node2 x = query(m + 1 - c[i].l);
            long long z;
            int id1;
            if (x.id1 == id)
                z = x.mx2, id1 = x.id2; 
            else
                z = x.mx1, id1 = x.id1;
            z = -z;
            if (z < mn[id]) mn[id] = z, to[id] = id1;
            upd(m + 1 - c[i].l, -(b[c[i].r] - b[c[i].l]), id);
        }
        fr(i, 1, m) t[i] = {-inf, -inf, 0, 0};
        rp(i, n, 1) {
            int id = fa[c[i].id];
            node2 x = query(c[i].r - 1);
            long long z;
            int id1;
            if (x.id1 == id)
                z = x.mx2, id1 = x.id2;
            else
                z = x.mx1, id1 = x.id1;
            if (id1) z = max(z, b[c[i].l]);
            z = b[c[i].r] - z;
            if (z < mn[id]) mn[id] = z, to[id] = id1;
            upd(c[i].l, b[c[i].l], id);
        }
        h.clear();
        fr(i, 1, n) if (fa[i] == i) h.push_back({i, to[i]});
        for (auto [x, y] : h) {
            int x1 = find(x), y1 = find(y);
            if (x1 == y1) continue;
            ans += mn[x];
            fa[x1] = y1;
            sz[y1] += sz[x1];
        }
    }
    printf("%lld\n", ans);
    return 0;
}