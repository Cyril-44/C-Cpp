#include <bits/stdc++.h>
constexpr int N = 200005;
constexpr int64_t INF = 1e18;
struct Node {
    int64_t t;
    int w, v;
} a[N], b[N];
struct CNode {
    int64_t t; 
    int w, v, p;
    bool typ;
    bool operator<(const CNode &o) const { return t < o.t || t == o.t && p < o.p; }
} c[N*2];
int64_t prewa[N], prewb[N], preva[N], prevb[N];
struct SegTr {
    int64_t mx[N << 2];
    int P; int64_t X;
    void pushup(int u) { mx[u] = std::max(mx[u<<1], mx[u<<1|1]); }
    void upd(int u, int l, int r) {
        if (l == r) mx[u] = X;
        else {
            int mid = l + r >> 1;
            P <= mid ? upd(u<<1, l, mid) : upd(u<<1|1, mid+1, r);
            pushup(u);
        }
    }
    int L, R;
    int64_t inq(int u, int l, int r) {
        if (L <= l && r <= R) return mx[u];
        int mid = l + r >> 1; int64_t res = 0;
        if (L <= mid) res = std::max(res, inq(u<<1, l, mid));
        if (mid < R) res = std::max(res, inq(u<<1|1, mid+1, r));
        return res;
    }
    int n;
    void update(int p, int64_t x) { P=p, X=x; upd(1, 1, n); }
    int64_t inquire(int l, int r) { if (l>r) return 0; L=l, R=r; return inq(1, 1, n); }
} fa, fb;
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++)
        scanf("%d%ld%d", &a[i].w, &a[i].t, &a[i].v), prewa[i] = prewa[i-1] + a[i].w;
    for (int i = 1; i <= m; i++)
        scanf("%d%ld%d", &b[i].w, &b[i].t, &b[i].v), prewb[i] = prewb[i-1] + b[i].w;
    if (a[1].v != 1) {
        for (int i = 1; i <= n; i++) preva[i] = preva[i-1] + a[i].v;
        for (int i = 1; i <= m; i++) prevb[i] = prevb[i-1] + b[i].v;
        const int64_t t = a[1].t;
        int64_t ans = -INF;
        for (int i = 0; i <= n; i++) {
            if (prewa[i] > t) break;
            int j = std::upper_bound(prewb+1, prewb+1+m, t - prewa[i]) - prewb-1;
            ans = std::max(ans, preva[i] + prevb[j]);
        }
        printf("%ld\n", ans);
        return 0;
    } else {
        fa.n = n, fb.n = m;
        for (int i = 1; i <= n; i++) c[i] = {a[i].t, a[i].w, a[i].v, i, 0};
        for (int i = 1; i <= m; i++) c[i+n] = {b[i].t, b[i].w, b[i].v, i, 1};
        std::sort(c+1, c+1+n+m);
        for (int i = 1; i <= n+m; i++) {
            if (c[i].w > c[i].t) continue;
            if (c[i].typ == 0) {
                int l = std::upper_bound(prewa+1, prewa+1+n, c[i].t - c[i].w) - prewa-1;
                int j = std::upper_bound(prewb+1, prewb+1+m, c[i].t - prewa[c[i].p]) - prewb-1;
                fa.update(c[i].p, c[i].v + std::max(fa.inquire(1, l), fb.inquire(1, j)));
            }
            if (c[i].typ == 0) {
                int l = std::upper_bound(prewb+1, prewb+1+m, c[i].t - c[i].w) - prewb-1;
                int j = std::upper_bound(prewa+1, prewa+1+n, c[i].t - prewb[c[i].p]) - prewa-1;
                fb.update(c[i].p, c[i].v + std::max(fb.inquire(1, l), fa.inquire(1, j)));
            }
        }
        printf("%ld\n", std::max(fa.inquire(1, n), fb.inquire(1, m)));
    }
}