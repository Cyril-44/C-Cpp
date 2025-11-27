#include <stdio.h>
#include <string.h>
constexpr int N = 500005, M = 1 << 20;
int f[M], g[N];
#define max(x, y) ((x) > (y) ? (x) : (y))
int main() {
    int n, k;
    scanf("%d%d", &n, &k);
    memset(f, -1, sizeof f);
    int sum = 0, ans = 0;
    f[0] = 0;
    for (int i = 1, ai; i <= n; i++) {
        scanf("%d", &ai);
        sum ^= ai;
        if (sum == k) g[i] = 1;
        g[i] = max(g[i], g[i-1]);
        if (~f[sum ^ k]) g[i] = max(g[i], g[f[sum ^ k]] + 1);
        ans = max(ans, g[i]);
        f[sum] = i;
        // fprintf(stderr, "%d %d %d\n", sum, f[sum ^ k], g[i]);
    }
    printf("%d\n", ans);
    return 0;
}