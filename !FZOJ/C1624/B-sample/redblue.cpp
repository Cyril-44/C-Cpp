#include <bits/stdc++.h>
constexpr int N = 100005;
#ifdef CLANGD
#define N 25
#endif
using Info = std::pair<int,int>;
class SegTr {
    struct Node {
        Info mn, mx; int add;
        void pull(int t) { mn.first += t, mx.first += t, add += t; }
    } tr[N*4 << 2];
    int L, R, X;
    void pushdown(int u) {
        tr[u<<1].pull(tr[u].add);
        tr[u<<1|1].pull(tr[u].add);
        tr[u].add = 0;
    }
    void pushup(int u) {
        tr[u].mn = std::min(tr[u<<1].mn, tr[u<<1|1].mn);
        tr[u].mx = std::max(tr[u<<1].mx, tr[u<<1|1].mx);
    }
    void upd(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u].pull(X);
        int mid = l + r >> 1;
        pushdown(u);
        if (L <= mid) upd(u<<1, l, mid);
        if (mid < R) upd(u<<1|1, mid+1, r);
        pushup(u);
    }
    void build(int u, int l, int r) {
        if (l == r) tr[u].mn = tr[u].mx = {0,l};
        else {
            int mid = l + r >> 1;
            build(u<<1, l, mid);
            build(u<<1|1, mid+1, r);
            pushup(u);
        }
    }
public:
    int n;
    void init(int _n) { build(1, 1, n = _n); }
    void update(int l, int r, int v) { L=l, R=r, X=v; upd(1, 1, n); }
    Info inquire() { return std::max(Info{-tr[1].mn.first, tr[1].mn.second}, tr[1].mx); }
} f;
struct Update {
    int x, l, r, v;
    bool operator<(const Update &o) const { return x < o.x; }
} a[N*4];
int all[N*4];
int main() {
    int n, m, w, h;
    scanf("%d%d%d%d", &n, &m, &w, &h);
    for (int i = 1, x, y; i <= n; i++) {
        scanf("%d%d", &x, &y);
        a[(i-1)*2+1] = {x, y, y+h, 1};
        a[(i-1)*2+2] = {x+w+1, y, y+h, -1};
        all[(i-1)*2+1] = y;
        all[(i-1)*2+2] = y+h;
    }
    for (int i = 1, x, y; i <= m; i++) {
        scanf("%d%d", &x, &y);
        a[n*2+(i-1)*2+1] = {x, y, y+h, -1};
        a[n*2+(i-1)*2+2] = {x+w+1, y, y+h, 1};
        all[n*2+(i-1)*2+1] = y;
        all[n*2+(i-1)*2+2] = y+h;
    }
    std::sort(all+1, all+1+n*2+m*2);
    f.init(std::unique(all+1, all+1+n*2+m*2) - all - 1);
    for (int i = 1; i <= n*2+m*2; i++) {
        a[i].l = std::lower_bound(all+1, all+1+f.n, a[i].l) - all;
        a[i].r = std::lower_bound(all+1, all+1+f.n, a[i].r) - all;
    }
    std::sort(a+1, a+1+n*2+m*2);
    int ans = -1, x = 0, y = 0;
    for (int i = 1; i <= n*2+m*2; i++) {
        f.update(a[i].l, a[i].r, a[i].v);
        if (a[i].x != a[i+1].x) {
            auto res = f.inquire();
            if (res.first > ans)
                ans = res.first, x = a[i].x - w, y = all[res.second] - h;
        }
    }
    printf("%d\n%d %d\n", ans, x, y);
}