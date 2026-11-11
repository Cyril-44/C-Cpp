#include <bits/stdc++.h>
constexpr int N = 300005;
#ifdef CLANGD
#define N 35
#endif
int n;
std::valarray<int> cyc[N];
class SegTrCyc {
    struct Node {
        std::valarray<int> a;
        int rot;
        Node() : a(8) {}
        void pull(int t) { a = a.cshift(t), rot += t; }
    } tr[N << 2];
    void pushup(int u) { tr[u].a = tr[u<<1].a + tr[u<<1|1].a; }
    void pushdown(int u) { if (tr[u].rot) tr[u<<1].pull(tr[u].rot), tr[u<<1|1].pull(tr[u].rot), tr[u].rot = 0; }
    int L, R;
    void set(int u, int l, int r) {
        if (l == r) { tr[u].a = cyc[l]; return; }
        int mid = l + r >> 1;
        pushdown(u);
        L <= mid ? set(u<<1, l, mid) : set(u<<1|1, mid+1, r);
        pushup(u);
    }
    void upd(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u].pull(1);
        int mid = l + r >> 1;
        pushdown(u);
        if (L <= mid) upd(u<<1, l, mid);
        if (mid < R) upd(u<<1|1, mid+1, r);
        pushup(u);
    }
    int inq(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u].a[0];
        int mid = l + r >> 1; int res = 0;
        pushdown(u);
        if (L <= mid) res += inq(u<<1, l, mid);
        if (mid < R) res += inq(u<<1|1, mid+1, r);
        return res;
    }
public:
    void add(int p) { L=p, set(1, 1, n); }
    void update(int l, int r) { L=l, R=r; upd(1, 1, n); }
    int inquire(int l, int r) { L=l, R=r; return inq(1, 1, n); }
    void print(int u=1, int l=1, int r=n) {
        if (l == r) printf("%lld%c", tr[u].a[0], " \n"[l==n]);
        else {
            int mid = l +r >> 1;
            pushdown(u);
            print(u<<1, l, mid), print(u<<1|1, mid+1, r); 
        }
    }
} fc;
int64_t a[N], hist[50];
int vis[1000];
inline int pow2(int x) { return x * x; }
inline int calc(int64_t x) {
    int res = 0;
    for (; x; x /= 10) res += pow2(x%10);
    return res;
}
class SegTrLin {
    constexpr static int MX = 12;
    struct Node {
        int64_t a[MX];
        int cap, red;
        void pull(int t) {
            red += t, cap -= t;
            if (cap <= MX)
                for (int i = 0; i < cap; i++)
                    a[i] = a[i + t];
        }
    } tr[N << 2];
    void pushup(int u) {
        tr[u].cap = std::min(tr[u<<1].cap, tr[u<<1|1].cap);
        for (int i = 0; i < tr[u].cap && i < MX; i++)
            tr[u].a[i] = tr[u<<1].a[i] + tr[u<<1|1].a[i];
    }
    void pushdown(int u) { if (tr[u].red) tr[u<<1].pull(tr[u].red), tr[u<<1|1].pull(tr[u].red), tr[u].red = 0; }
    int L, R;
    void renew(int u, int l, int r) {
        if (tr[u].cap) return;
        if (l == r) {
            tr[u].cap = 1e9;
            memset(tr[u].a, 0, sizeof tr[u].a);
            return fc.add(l);
        }
        int mid = l + r >> 1;
        pushdown(u);
        renew(u<<1, l, mid);
        renew(u<<1|1, mid+1, r);
        pushup(u);
    }
    void upd(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u].pull(1);
        int mid = l + r >> 1;
        pushdown(u);
        if (L <= mid) upd(u<<1, l, mid);
        if (mid < R) upd(u<<1|1, mid+1, r);
        pushup(u);
    }
public:
    void build(int u=1, int l=1, int r=n) {
        if (l == r) {
            memset(vis, -1, sizeof vis);
            hist[0] = a[l];
            int x = calc(a[l]);
            for (int i = 1; ; i++, x = calc(x)) {
                if (~vis[x]) {
                    tr[u].cap = vis[x];
                    for (int j = 0; j < vis[x]; j++)
                        tr[u].a[j] = hist[j];
                    cyc[l].resize(8);
                    for (int j = vis[x]; j < i; j++)
                        cyc[l][j - vis[x]] = hist[j];
                    if (x == 1) cyc[l] = 1;
                    assert(i - vis[x] == 8 || i - vis[x] == 1);
                    break;
                }
                hist[i] = x, vis[x] = i;
            }
            return;
        }
        int mid = l + r >> 1;
        build(u<<1, l, mid), build(u<<1|1, mid+1, r);
        pushup(u);
    }
    int64_t inq(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u].a[0];
        int mid = l + r >> 1; int64_t res = 0;
        pushdown(u);
        if (L <= mid) res += inq(u<<1, l, mid);
        if (mid < R) res += inq(u<<1|1, mid+1, r);
        return res;
    }
    void update(int l, int r) { L=l, R=r; upd(1, 1, n), renew(1, 1, n); }
    int64_t inquire(int l, int r) { L=l, R=r; return inq(1, 1, n); }
    void print(int u=1, int l=1, int r=n) {
        if (l == r) printf("%lld%c", tr[u].a[0], " \n"[l==n]);
        else {
            int mid = l + r >> 1;
            pushdown(u);
            print(u<<1, l, mid), print(u<<1|1, mid+1, r); 
        }
    }
} fl;
int main() {
    freopen("digit.in", "r", stdin);
    freopen("digit.out", "w", stdout);
    int q;
    scanf("%d%d", &n, &q);
    for (int i = 1; i <= n; i++)
        scanf("%lld", &a[i]);
    fl.build();
    for (int op, l, r; q--; ) {
        scanf("%d%d%d", &op, &l, &r);
        if (op == 1) fc.update(l, r), fl.update(l, r);
        else printf("%lld\n", fc.inquire(l, r) + fl.inquire(l, r));
        // fl.print(), fc.print();
    }
    return 0;
}