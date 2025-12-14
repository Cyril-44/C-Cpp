#include <cstdio>
#include <algorithm>
using std::min;
const int N = 1005;
int a[N], f[N][N], g[N][N], h[N][N];
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &a[i]);
        f[i][i] = g[i][i] = h[i][i] = a[i];
        a[i] += a[i - 1];
    }
    for (int k = 1; k < n; k++)
        for (int l = 1, r; l+k <= n; l++) {
            r = l + k;
            f[l][r] = a[r] - a[l-1] - min(0, min(g[l+1][r], h[l][r-1]));
            g[l][r] = min(f[l][r], g[l+1][r]);
            h[l][r] = min(f[l][r], h[l][r-1]);
        }
    printf("%lld\n", f[1][n] * 2ll - a[n]);
    return 0;
}