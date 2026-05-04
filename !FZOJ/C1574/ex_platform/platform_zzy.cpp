#include <bits/stdc++.h>
#define ll long long
#define pr pair<int, int>
#define mk make_pair
#define fi first
#define se second
#pragma GCC optimize(2)
#pragma GCC optimize(3)
#pragma GCC optimize("Ofast")
using namespace std;
template <typename T> void in(T &x) {
    char c = getchar();
    while (!isdigit(c)) c = getchar();
    for (x = 0; isdigit(c); c = getchar()) x = x * 10 + c - '0';
}
const int N = 2e5 + 5, inf = 2e9 + 7;
int n, m, b[N];
int mn[N], id[N], fa[N], ys[N];
ll ans;
struct h {
    int l, r, id;
} a[N];
bool cmpl(h a, h b) {
    return a.l ^ b.l ? a.l < b.l : a.r < b.r;
}
bool cmpr(h a, h b) {
    return a.r ^ b.r ? a.r > b.r : a.l > b.l;
}
bool cmpr2(h a, h b) {
    return a.r ^ b.r ? a.r < b.r : a.l > b.l;
}
int fr(int u) {
    return u == fa[u] ? u : fa[u] = fr(fa[u]);
}
void cmn(int i, int a, int b) {
    if (a < mn[i]) mn[i] = a, id[i] = b;
}
bool join(int x, int y) {
    x = fr(x), y = fr(y);
    if (x == y) return 0;
    fa[x] = y;
    return 1;
}
struct Max {
    int m1, m2, i1, i2;
} tr[4 * N], tmp, e;
void cl(Max &a, int x, int id) {
    if (id == a.i1)
        a.m1 = max(a.m1, x);
    else {
        if (x > a.m1)
            a.m2 = a.m1, a.i2 = a.i1, a.m1 = x, a.i1 = id;
        else if (x > a.m2)
            a.m2 = x, a.i2 = id;
    }
}
Max operator+(Max a, Max b) {
    cl(a, b.m1, b.i1), cl(a, b.m2, b.i2);
    return a;
}
struct BIT {
    Max tr[N];
    void build() {
        for (int i = 1; i <= m; ++i) tr[i] = e;
    }
    void upd(int x, int a, int b) {
        for (; x <= m; x += x & -x) cl(tr[x], a, b);
    }
    pr que(int x, int id) {
        Max tmp = e;
        for (; x; x -= x & -x) tmp = tmp + tr[x];
        return tmp.i1 == id ? mk(tmp.m2, tmp.i2) : mk(tmp.m1, tmp.i1);
    }
} A;
int main() {
    e = (Max){-inf, -inf, -1, -1};
    in(n);
    for (int i = 1; i <= n; ++i) {
        in(a[i].l), in(a[i].r), a[i].id = i;
        fa[i] = i, b[++m] = a[i].l, b[++m] = a[i].r;
    }
    sort(b + 1, b + 1 + m), m = unique(b + 1, b + 1 + m) - b - 1;
    for (int i = 1; i <= n; ++i)
        a[i].l = lower_bound(b + 1, b + 1 + m, a[i].l) - b, a[i].r = lower_bound(b + 1, b + 1 + m, a[i].r) - b;
    for (;;) {
        for (int i = 1; i <= n; ++i) mn[i] = inf, id[i] = 0;
        sort(a + 1, a + 1 + n, cmpl);
        A.build();
        for (int i = 1; i <= n; ++i) {
            pr w = A.que(m - a[i].l, fr(a[i].id));
            if (w.fi > -inf) cmn(fr(a[i].id), min(-w.fi, b[a[i].r]) - b[a[i].l], w.se);
            A.upd(m - a[i].r + 1, -b[a[i].r], fr(a[i].id));
        }
        sort(a + 1, a + 1 + n, cmpr);
        A.build();
        for (int i = 1; i <= n; ++i) {
            pr w = A.que(a[i].r - 1, fr(a[i].id));
            if (w.fi > -inf) cmn(fr(a[i].id), b[a[i].r] - max(b[a[i].l], w.fi), w.se);
            A.upd(a[i].l, b[a[i].l], fr(a[i].id));
        }
        reverse(a + 1, a + 1 + n);
        A.build();
        for (int i = 1; i <= n; ++i) {
            pr w = A.que(m - a[i].l + 1, fr(a[i].id));
            cmn(fr(a[i].id), -w.fi, w.se);
            A.upd(m - a[i].l + 1, b[a[i].l] - b[a[i].r], fr(a[i].id));
        }
        for (int i = 1; i <= n; ++i)
            if (i == fa[i] && join(id[i], i)) ans += mn[i];
        int cnt = 0;
        for (int i = 1; i <= n; ++i)
            if (fa[i] == i) cnt++;
        if (cnt == 1) break;
    }
    printf("%lld\n", ans);
    return 0;
}