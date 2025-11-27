#include <stdio.h>
#include <algorithm>
#include <limits>
const int N = 100005;
struct RainDrop {
    int x, y;
    inline bool operator<(const RainDrop &oth) const {
        return x < oth.x;
    }
} a[N];
int mn[N], mx[N];
int main() {
    int n, d;
    scanf("%d%d", &n, &d);
    for (int i = 0; i < n; i++)
        scanf("%d%d", &a[i].x, &a[i].y);
    std::sort(a, a + n);
    int l(0), xl(1), xr(0), nl(1), nr(0), ans(std::numeric_limits<int>::max());
    for (int r = 0; r < n; r++) {
        while (xl <= xr && a[mx[xr]].y <= a[r].y) --xr;
        while (nl <= nr && a[mn[nr]].y >= a[r].y) --nr;
        mx[++xr] = r, mn[++nr] = r;
        while (l < r && a[mx[xl]].y - a[mn[nl]].y >= d) {
            ans = std::min(ans, a[r].x - a[l].x);
            ++l;
            while (xl <= xr && a[mx[xl]].x < a[l].x) ++xl;
            while (nl <= nr && a[mn[nl]].x < a[l].x) ++nl;
        }
    }
    printf("%d\n", (ans ^ std::numeric_limits<int>::max()) ? ans : -1);
    return 0;
}