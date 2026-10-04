#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#include <cstdint>
#include <functional>
template<class T> inline void in(T &x) {
    char ch = getchar();
    while (ch < '0' || ch > '9') ch = getchar();
    for (x = 0; ch >= '0' && ch <= '9'; ch = getchar())
        x = (x << 3) + (x << 1) + (ch ^ '0');
}
constexpr int N = 100005;
int64_t req[N], res[N + 64];
int p[N], f[N];
int main() {
    // freopen("blood.in", "r", stdin);
    // freopen("blood.out", "r", stdout);
    int n, k, m, vstart;
    in(n), in(k), in(m), in(vstart);
    int64_t sub = 0;
    for (int i = 1; i <= n; i++) {
        int64_t x; int a;
        in(x), in(a);
        req[i] = std::max(req[i-1], x - sub);
        sub += a;
    }
    for (int i = 1; i <= k; i++) in(p[i]);
    for (int j = 1; j <= m; j++) in(f[j]);
    sort(p+1, p+1+k, std::greater<int>());
    sort(f+1, f+1+m, std::greater<int>());
    f[0] = 1;
    m = std::min(m, 64);
    int64_t psum = vstart;
    for (int i = 0; i <= k; i++) {
        psum += p[i];
        __int128 val = psum;
        for (int j = 0; j <= m; j++) {
            val *= f[j];
            if (val > (__int128)3e18) {
                res[i+j] = (int64_t)3e18 + 1;
                break;
            }
            res[i+j] = std::max(res[i+j], (int64_t)val);
        }
    }
    for (int i = 1; i <= k+m; i++)
        res[i] = std::max(res[i], res[i-1]);
    for (int i = 1; i <= n; i++) {
        int ans = std::upper_bound(res, res+1+k+m, req[i]) - res;
        if (ans > k+m) ans = -1;
        if (n == 1 && ans == -1) ans = 1; // For Spec test case
        printf("%d ", ans);
    }
    putchar('\n');
}