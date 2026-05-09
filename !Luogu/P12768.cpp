#include <cstdio>
#include <algorithm>
#include <limits>
#include <numeric>
constexpr int N = 500005;
int a[N], b[N], c[N], p[N], q[N], fa[N], n;
inline int find(int u) { return fa[u] ^ u ? fa[u] = find(fa[u]) : u; }
inline bool unite(int u, int v) {
    u = find(u), v = find(v);
    return (fa[u] ^ fa[v]) ? (fa[u] = v) : false;
}
struct SegTr {
    int mn[N << 2], arr[N];
    void build(int u, int l, int r) {
        if (l == r) mn[u] = arr[l];
        else {
            int mid = l + r >> 1;
            build(u << 1, l, mid);
            build(u << 1 | 1, mid + 1, r);
            pushup(u);
        }
    }
    SegTr(int *_a, int *_b) {
        for (int i = 1; i <= n; i++)
            arr[_a[i]] = _b[i];
        build(1, 1, n);
    }
    inline void pushup(int u) { mn[u] = std::min(mn[u<<1], mn[u<<1|1]); }
    inline void del(int p) {
        int u = 1, l = 1, r = n;
        static int sta[30];
        sta[0] = 0;
        while (l != r) {
            sta[++sta[0]] = u;
            int mid = l + r >> 1;
            if (p <= mid) r = mid, u = u<<1;
            else l = mid+1, u = u<<1|1;
        }
        mn[u] = std::numeric_limits<int>::max();
        while (sta[0]) pushup(sta[sta[0]--]);
    }
    inline int inquire(int p) {
        int res = std::numeric_limits<int>::max();
        int u = 1, l = 1, r = n;
        while (l != r) {
            int mid = l + r >> 1;
            if (p <= mid) r = mid, u = u<<1;
            else res = std::min(res, mn[u<<1]), l = mid+1, u = u<<1|1;
        }
        res = std::min(res, mn[u]);
        return res;
    }
};
int main() {
    int m;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) scanf("%d", a+i);
    for (int i = 1; i <= n; i++) scanf("%d", b+i);
    for (int i = 1; i <= n; i++) scanf("%d", c+i);
    std::iota(p+1, p+1+n, 1);
    std::stable_sort(p+1, p+1+n, [](int x, int y) { return (a[x]<a[y]) + (b[x]<b[y]) + (c[x]<c[y]) < 2; });
    for (int i = 1; i <= n; i++) q[p[i]] = i;
    SegTr f1(a, b), f2(a, c), f3(b, c);
    std::iota(fa+1, fa+1+n, 1);
    for (int i = n, j = n; i >= 1; i--) {
        for (; j >= 1; j--) {
            if (f1.inquire(a[p[i]]) > b[p[i]] && f2.inquire(a[p[i]]) > c[p[i]] && f3.inquire(b[p[i]]) > c[p[i]]) break;
            unite(i, j);
            f1.del(a[p[j]]), f2.del(a[p[j]]), f3.del(b[p[j]]);
        }
    }
    scanf("%d", &m);
    for (int x, y; m--; ) {
        scanf("%d%d", &x, &y);
        puts(find(q[x]) >= find(q[y]) ? "TAK" : "NIE");
    }
    return 0;
}