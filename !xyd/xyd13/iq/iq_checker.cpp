#include <bits/stdc++.h>
#include <random>
using namespace std;
#define For(i,s,t) for (int i = (s); (i) <= (t); ++(i))
#ifdef DEBUG
# define debug(...) fprintf(stderr, __VA_ARGS__)
#else
# define debug(...) (void(0))
#endif
constexpr int N = 1005, MX = 1e9;
int n, k, p0;
int a[N];
namespace My{
    int a[N];
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
    inline int solveTestCase() {
        std::sort(a+1, a+1 + n);
        int ans1 = solve();
        For (i, 1, n) a[i] = MX - a[i];
        p0 = MX - p0 - k;
        std::reverse(a+1, a+1 + n);
        int ans2 = solve();
        return std::max(ans1, ans2);
    }
}
namespace Std {
    int a[N];
    int solve() {
        int ans = 0;
        int pL = std::lower_bound(a + 1, a + n + 1, p0) - a,
            pR = std::lower_bound(a + 1, a + n + 1, p0 + k + 1) - a - 1;
        For(i, 0, n - pR) {
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
                0, pL - std::max(r + 1, 
                    (int)(std::lower_bound(
                        a + 1, a + n + 1, R - k - r) -
                    a)));
            ans = std::max(ans, sum);
        }
        return ans;
    }

    int main() {
        int ans = solve();
        For(i, 1, n) a[i] = (int)1e9 - a[i];
        p0 = (int)1e9 - p0 - k;
        std::reverse(a + 1, a + n + 1);
        ans = std::max(ans, solve());
        return ans;
    }
}
int main() {
    mt19937_64 rng(time(nullptr));
    int my, std, p0;
start:
    n = uniform_int_distribution<int>(1, 10)(rng);
    k = uniform_int_distribution<int>(1, 10)(rng);
    p0 = uniform_int_distribution<int>(1, 10)(rng);
    For (i, 1, n) a[i] = uniform_int_distribution<int>(1, 10)(rng);
    memcpy(My::a, a, sizeof(int) * (n+1));
    memcpy(Std::a, a, sizeof(int) * (n+1));
    ::p0 = p0;
    std = Std::main();
    ::p0 = p0;
    my = My::solveTestCase();
    if (my != std) {
        sort(a+1, a+1 + n);
        freopen("iq_hack.in", "w", stdout);
        printf("%d %d %d\n", n, k, p0);
        For (i, 1, n) printf("%d\n", a[i]);
        freopen("iq_hack.ans", "w", stdout);
        printf("%d\nFound %d\n", std, my);
        fclose(stdout);
        throw;
    }
    printf("Accepted, n=%d, k=%d, p0=%d\n", n, k, p0);
goto start;
    return 0;
}