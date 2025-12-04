#include <stdio.h>
#include <string.h>
#include <algorithm>
#ifdef DEBUG
#define debug(...) fprintf(stderr, __VA_ARGS__)
#else
#define debug(...) ((void)0)
#endif
// xi > xj && yi < yj
constexpr int N = 100005;
struct Node { int x, y, id; } a[N];
int p[N], n, k;
namespace BIT {
    int tr[N];
    inline void upd(int p, int x) {
        for (; p <= n; p += p & -p) tr[p] += x;
    }
    inline int sum(int p) {
        int s = 0;
        for (; p > 0; p -= p & -p) s += tr[p];
        return s;
    }
}
long long ans = 0;
void cdq1(int l, int r) { // 确保 xl > xr, 计算 yl < yr
    if (l == r) return;
    int mid = l + r >> 1;
    cdq1(l, mid), cdq1(mid + 1, r);
    std::sort(a+l, a+r+1, [](const Node&x, const Node&y){return x.x > y.x;});
    std::sort(a+l, a+mid+1, [](const Node&x, const Node&y){return x.y < y.y;});
    std::sort(a+mid+1, a+r+1, [](const Node&x, const Node&y){return x.y < y.y;});
    int i = l;
    for (int j = mid+1; j <= r; ++j) {
        while (i <= mid && a[i].y <= a[j].y) BIT::upd(a[i++].id, 1);
        ans += BIT::sum(n) - (BIT::sum(std::min(a[j].id + k, n)) - BIT::sum(std::max(a[j].id - k - 1, 0)));
        debug("%d %d %d %d(%d) %d(%d)\n", i, j, a[j].id, BIT::sum(std::min(a[j].id + k, n)), std::min(a[j].id + k, n), BIT::sum(std::max(a[j].id - k, 1)), std::max(a[j].id - k, 1));
    }
    while (--i >= l) BIT::upd(a[i].id, -1);
}
void cdq2(int l, int r) { // 确保 yl > yr, 计算 xl < xr
    if (l == r) return;
    int mid = l + r >> 1;
    cdq2(l, mid), cdq2(mid + 1, r);
    std::sort(a+l, a+r+1, [](const Node&x, const Node&y){return x.y > y.y;});
    std::sort(a+l, a+mid+1, [](const Node&x, const Node&y){return x.x < y.x;});
    std::sort(a+mid+1, a+r+1, [](const Node&x, const Node&y){return x.x < y.x;});
    int i = l;
    for (int j = mid+1; j <= r; ++j) {
        while (i <= mid && a[i].x <= a[j].x) BIT::upd(a[i++].id, 1);
        ans += BIT::sum(n) - (BIT::sum(std::min(a[j].id + k, n)) - BIT::sum(std::max(a[j].id - k - 1, 0)));
        // debug("%d %d %d %d(%d) %d(%d)\n", i, j, a[j].id, BIT::sum(std::min(a[j].id + k, n)), std::min(a[j].id + k, n), BIT::sum(std::max(a[j].id - k, 1)), std::max(a[j].id - k, 1));
    }
    while (--i >= l) BIT::upd(a[i].id, -1);
}
int main() {
    scanf("%d%d", &n, &k);
    for (int i = 1; i <= n; i++) a[i].id = i;
#define READ(val) \
    for (int i = 1; i <= n; i++) scanf("%d", &p[i]); \
    for (int i = 1; i <= n; i++) a[p[i]].val = i
    READ(x); READ(y);
    cdq1(1, n);
    cdq2(1, n);
    printf("%lld\n", ans);
    return 0;
}
