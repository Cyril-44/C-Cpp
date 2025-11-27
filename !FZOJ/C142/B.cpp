#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <functional>
namespace FastI {
    char buf[1 << 20], *p1=nullptr, *p2=nullptr;
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
constexpr int N = 505, M = 1000005;
using LL = long long;
struct Orig {
    int a, b;
    inline bool operator< (const Orig &rhs) const {
        return a > rhs.a;
    }
} pl[N];
int c[M];
LL f[N][N];
int main() {
#ifndef ONLINE_JUDGE
    freopen("fief1.in", "r", stdin);
    freopen("fief.out", "w", stdout);
#endif
    int n, m, K;
    in(n), in(m), in(K);
    LL ans = 0;
    for (int i = 1; i <= n; i++) {
        in(pl[i].a), in(pl[i].b);
        ans += (LL)pl[i].a * pl[i].b;
    }
    std::sort(pl+1, pl+1 + n);
    for (int i = 1; i <= m; i++) in(c[i]);
    std::sort(c+1, c+1 + m, std::greater<int>());
    m = std::min(n, m);
    for (int i = 1; i <= m; i++) {
        memset(f, 0, sizeof f);
        // for (int j = 1; j <= n; j++)
        //     g[j] = g[j-1] + std::max((LL)pl[j].a * (pl[j].b - (LL)i * k), 0LL);
        for (int j = 1; j <= n; j++) { // 在 j 位置
            int x = pl[j].a, y = pl[j].b;
            f[j][0] = f[j-1][0] + std::max((LL)x * (y - (LL)i * K), 0LL);
            for (int k = 1; k <= i; k++) { // 使用前 k 个领主 
                f[j][k] = std::max(
                    f[j-1][k-1] + std::max(x * (c[k] - (k-1ll) * K), 0LL),
                    f[j-1][k] + std::max(x * (y - (LL)i * K), 0LL)
                );
            }
        }
        /* for (int j = 1; j <= i; j++) { // 使用了前 j 个 领主
            f[j][0][0] = f[j-1][0][0];
            for (int k = 1; k <= n; k++) { // 当前在 k 的位置
                f[j][k][1] = f[j-1][k-1][0] + std::max((LL)pl[k].a * (c[j] - (LL)(j - 1) * K), 0LL);
                f[j][k][0] = std::max(
                    f[j][k-1][0] + std::max((LL)pl[k].a * (pl[k].b - (LL)i * K), 0LL),
                    f[j][k][1]
                );
            }
        } */
        // fprintf(stderr, "%lld\n", std::max(f[i][n][0], f[i][n][1]));
        ans = std::max(ans, f[n][i]);
        // ans = std::max(ans, std::max(f[n][i][0], f[n][i][1]));
    }
    printf("%lld\n", ans);
    return 0;
}
