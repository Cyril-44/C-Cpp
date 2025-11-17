#include <stdio.h>
#include <algorithm>
#include <string.h>
#include <math.h>
constexpr int N = 500005;
using Range = std::pair<int, int>;
#define L first
#define R second
Range rg[N];
int x[N];
bool h[N];
int main() {
    freopen("badge.in", "r", stdin);
    freopen("badge.out", "w", stdout);
    int n, m, q;
    scanf("%d%d", &n, &q);
    for (int i = 1; i <= n; i++)
        scanf("%d%d", &rg[i].L, &rg[i].R);
    std::sort(rg+1, rg+1 + n);
    int blk = sqrt(n);
    while (q--) {
        scanf("%d", &m);
        for (int i = 1; i <= m; i++) scanf("%d", x+i);
        std::sort(x+1, x+1 + m);
        if (m > blk) { // Brute force
            memset(h, 0, n+1);
            for (int i = 1; i <= m; i++) h[x[i]] = 1;
            for (int i = 1; i <= n; i++) h[i] ^= h[i-1];
            int ans = 0;
            for (int i = 1; i <= n; i++)
                ans += h[rg[i].R] ^ h[rg[i].L - 1];
            printf("%d\n", ans);
        } else {

        }
    }
    return 0;
}