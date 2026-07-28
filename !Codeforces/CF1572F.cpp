#include <cstdio>
#include <cstring>
#include <cstdint>
#include <algorithm>
#include <cassert>
constexpr int N = 200005;
int n;
class SegTr {
    int64_t sum[N<<2], add[N<<2];
    int len[N<<2], L, R, X;
    void pushup(int u) {
        sum[u] = sum[u<<1] + sum[u<<1|1];
    }
    void pushdown(int u) {
        if (add[u]) {
            add[u<<1] += add[u], add[u<<1|1] += add[u];
            sum[u<<1] += add[u] * len[u<<1], sum[u<<1|1] += add[u] * len[u<<1|1];
            add[u] = 0;
        }
    }
    int64_t inq(int u, int l, int r) {
        if (L <= l && r <= R) return sum[u];
        int mid = l + r >> 1;
        int64_t res = 0;
        pushdown(u);
        if (L <= mid) res += inq(u<<1, l, mid);
        if (mid < R) res += inq(u<<1|1, mid+1, r);
        return res;
    }
    void upd(int u, int l, int r) {
        if (L <= l && r <= R) sum[u] += X * len[u], add[u] += X;
        else {
            int mid = l + r >> 1;
            pushdown(u);
            if (L <= mid) upd(u<<1, l, mid);
            if (mid < R) upd(u<<1|1, mid+1, r);
            pushup(u);
        }
    }
public:
    void build(int u=1, int l=1, int r=n) {
        len[u] = r - l + 1;
        if (l == r) sum[u] = 1;
        else {
            sum[u] = len[u];
            int mid = l + r >> 1;
            build(u<<1, l, mid);
            build(u<<1|1, mid+1, r);
        }
    }
    int64_t inquire(int l, int r) { L=l, R=r; return inq(1, 1, n); }
    void update(int l, int r, int x) { L=l, R=r, X=x; upd(1, 1, n); }
} fb; // 维护 b 序列
class SegBeats {
    struct Node {
        int mx, cnt, mx2, tag;
        void pull(int v, bool f = false) {
            if (f || mx > v && v > mx2) {
                if (f) fb.update(v+1, mx, -cnt);
                mx = v, tag = v;
            }
        }
    } tr[N << 2];
    void pushdown(int u) {
        if (~tr[u].tag) {
            tr[u<<1].pull(tr[u].tag);
            tr[u<<1|1].pull(tr[u].tag);
            tr[u].tag = -1;
        }
    }
    void pushup(int u) {
        if (tr[u<<1].mx == tr[u<<1|1].mx) tr[u] = {
            tr[u<<1].mx,
            tr[u<<1].cnt + tr[u<<1|1].cnt,
            std::max(tr[u<<1].mx2, tr[u<<1|1].mx2),
        -1}; else if (tr[u<<1].mx > tr[u<<1|1].mx) tr[u] = {
            tr[u<<1].mx,
            tr[u<<1].cnt,
            std::max(tr[u<<1].mx2, tr[u<<1|1].mx),
        -1}; else tr[u] = {
            tr[u<<1|1].mx,
            tr[u<<1|1].cnt,
            std::max(tr[u<<1].mx, tr[u<<1|1].mx2),
        -1};
    }
    int L, R, X, P;
    void chkmn(int u, int l, int r) {
        if (X >= tr[u].mx) return;
        if (L <= l && r <= R && tr[u].mx > X && X > tr[u].mx2) tr[u].pull(X, true);
        else {
            int mid = l + r >> 1;
            pushdown(u);
            if (L <= mid) chkmn(u<<1, l, mid);
            if (mid < R) chkmn(u<<1|1, mid+1, r);
            pushup(u);
        }
    }
    void upd(int u, int l, int r) {
        if (l == r) tr[u].mx = X;
        else {
            int mid = l + r >> 1;
            pushdown(u);
            if (P <= mid) upd(u<<1, l, mid);
            else upd(u<<1|1, mid+1, r);
            pushup(u);
        }
    }
public:
    void build(int u=1, int l=1, int r=n) {
        if (l == r) tr[u] = {l, 1, -1, -1};
        else {
            int mid = l + r >> 1;
            tr[u] = {r, 1, r-1, -1};
            build(u<<1, l, mid);
            build(u<<1|1, mid+1, r);
        }
    }
    void checkmin(int l, int r, int x) { L=l, R=r, X=x; chkmn(1, 1, n); }
    void apply(int p, int x) { P=p, X=x; upd(1, 1, n); }
} f;
int main() {
    int q;
    scanf("%d%d", &n, &q);
    fb.build(); f.build();
    for (int p, x, y, _q = 1; _q <= q; _q++) {
        scanf("%d%d%d", &p, &x, &y);
        if (p == 1) { // chkmin f[1,x] ==> x-1, set f[x]=y
            f.checkmin(1, x, x-1);
            f.apply(x, y);
            fb.update(x, y, 1);
        } else {
            printf("%lld\n", fb.inquire(x, y));
        }
    }
    return 0;
}