#include <stdio.h>
#include <algorithm>
#define For(i, s, t) for (int i = (s); (i) <= (t); ++(i))
constexpr int N = 1000005;
int a[N];
namespace FastI {
char buf[1 << 21], *p1(nullptr), *p2(nullptr);
inline char get() {
    if (p1 == p2)
        p2 = (p1 = buf) + fread(buf, 1, sizeof buf, stdin);
    return *p1++;
}
inline void in(int &x) {
    char ch = get();
    while (ch < '0' || ch > '9')
        ch = get();
    for (x = ch ^ '0', ch = get(); ch >= '0' && ch <= '9'; ch = get())
        x = (x << 3) + (x << 1) + (ch ^ '0');
}
} // namespace FastI
using FastI::in;
int n, k, p0;
int solve() {
    int ans = 0;
    int pL = std::lower_bound(a + 1, a + n + 1, p0) - a,
        pR = std::lower_bound(a + 1, a + n + 1, p0 + k + 1) - a - 1;
    For (i, 0, n - pR) {
        int R = std::max(p0 + k, a[pR + i]);
        int sum = pR + i - pL + 1;
        if (R - p0 - k > n - pR - i)
            break;
        int l = 1, r = pL, m;
        while (l <= r) {
            m = l + r >> 1;
            if (a[m] <= R - k - m)
                l = m + 1;
            else
                r = m - 1;
        }
        sum += std::max(
            0, pL - std::max(r + 1, (int)(std::lower_bound(a + 1, a + n + 1,
                                                           R - k - r) -
                                          a)));
        ans = std::max(ans, sum);
    }
    return ans;
}
int main() {
    freopen("iq.in", "r", stdin);
    freopen("iq.out", "w", stdout);
    int T;
    in(T);
    while (T--) {
        in(n), in(k), in(p0);
        For(i, 1, n) in(a[i]);
        std::sort(a + 1, a + n + 1);
        int ans = solve();
        For(i, 1, n) a[i] = (int)1e9 - a[i];
        p0 = (int)1e9 - p0 - k;
        std::reverse(a + 1, a + n + 1);
        ans = std::max(ans, solve());
        printf("%d\n", ans);
    }
    return 0;
}