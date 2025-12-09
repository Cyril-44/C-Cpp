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
void cdq(int l, int r) { // 确保 xl > xr, 计算 yl < yr
    if (l == r) return;
    int mid = l + r >> 1;
    cdq(l, mid), cdq(mid + 1, r);
    int i = l;
    for (int j = mid+1; j <= r; ++j) {
        while (i <= mid && a[i].y <= a[j].y) {
            debug("add %d(%d,%d)\n", a[i].id, a[i].x, a[i].y);
            BIT::upd(a[i++].id, 1);
        }
        ans += BIT::sum(n) - (BIT::sum(std::min(a[j].id + k, n)) - BIT::sum(std::max(a[j].id - k - 1, 0)));
        debug("set %d(%d,%d) ==> %d - %d + %d\n", a[j].id, a[j].x, a[j].y, BIT::sum(n), BIT::sum(std::min(a[j].id + k, n)), BIT::sum(std::max(a[j].id - k, 1)));
    }
    while (--i >= l) BIT::upd(a[i].id, -1);
    std::inplace_merge(a+l, a+mid+1, a+r+1, [](const Node&i, const Node&j){return i.y < j.y;});
}
int main() {
    scanf("%d%d", &n, &k);
    for (int i = 1; i <= n; i++) a[i].id = i;
#define READ(val) for (int i = 1, tp; i <= n; i++) scanf("%d", &tp), a[tp].val = i;
    READ(x); READ(y);
    std::sort(a+1, a+1 + n, [](const Node&i, const Node&j){return i.x > j.x;});
    cdq(1, n);
    printf("%lld\n", ans);
    return 0;
}
