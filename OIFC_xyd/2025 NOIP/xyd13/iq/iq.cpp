#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <tuple>
#define For(i,s,t) for (int i = (s); (i) <= (t); ++(i))
constexpr int N = 1000005, MX = 1e9;
int a[N];
#ifdef DEBUG
# define debug(...) fprintf(stderr, __VA_ARGS__)
#else
# define debug(...) (void(0))
#endif
namespace FastI {
    char buf[1 << 21], *p1(nullptr), *p2(nullptr);
    inline char get() {
        if (p1 == p2) p2 = (p1=buf) + fread(buf, 1, sizeof buf, stdin);
        return *p1++;
    }
    inline void in(int &x) {
        char ch = get();
        while (ch < '0' || ch > '9') ch = get();
        for (x = ch ^ '0', ch = get(); ch >= '0' && ch <= '9'; ch = get())
            x = (x << 3) + (x << 1) + (ch ^ '0');
    }
} using FastI::in;
int n, k, p0;
inline int solve() {
    int pL = std::lower_bound(a+1, a+1 + n, p0) - a,
        pR = std::upper_bound(a+1, a+1 + n, p0 + k) - a-1;
    debug("p0=%d, [%d,%d]\n", p0, pL, pR);
    int now = pL; // 当前最左边到的
    int lp = 1;   // 最左边未取的
    int rgL = p0; // 最左边在 [rgL, rgL + k]
    int ans = 0;
    while (now >= lp) {
        int l = pR, r = n, mid;
        while (l <= r) {
            mid = l + r >> 1;
            //  所需要的最少右移次数     可以提供的最大右移次数
            if (a[mid] - (p0 + k) <= n - mid - (pL - now)) l = mid + 1;
            else r = mid - 1;
        }
        ans = std::max(ans, r - now + 1);
        debug("%d %d [%d,%d]\n", lp, rgL, now, r);
        --now;
        lp += rgL - a[now], rgL = a[now];
    }
    return ans;
}
inline void solveTestCase() {
    in(n), in(k), in(p0);
    For (i, 1, n) in(a[i]);
    std::sort(a+1, a+1 + n);
    int ans1 = solve();
    For (i, 1, n) a[i] = MX - a[i];
    p0 = MX - p0 - k;
    std::reverse(a+1, a+1 + n);
    int ans2 = solve();
    printf("%d\n", std::max(ans1, ans2));
}
int main() {
    freopen("iq.in", "r", stdin);
    freopen("iq.out", "w", stdout);
#ifdef DEBUG
    freopen("iq.txt", "w", stderr);
#endif
    int T;
    in(T);
    For(i, 1, T) {
        debug("Case %d:\n", i);
        solveTestCase();
    }
    return 0;
}
