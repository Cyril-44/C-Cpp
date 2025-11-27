#include <stdio.h>
#include <string.h>
#include <algorithm>
constexpr int N = 1005;
#ifdef DEBUG
# define debug(...) fprintf(stderr, __VA_ARGS__)
#else
# define debug(...) ((void)0)
#endif
int a[N][N];
inline void in(int &x) {
    char ch = getchar();
    while ((ch ^ '0') && (ch ^ '1')) ch = getchar();
    x = ch & 1 ^ 1;
}
int sta[N], sum[N];
int main() {
    freopen("plant.in", "r", stdin);
    freopen("plant.out", "w", stdout);
    int T, id, n, m;
    scanf("%d%d", &T, &id);
    while (T--) {
        scanf("%d%d", &n, &m);
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= m; j++)
                in(a[i][j]);
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= m; j++)
                a[i][j] = a[i][j] ? a[i][j] + a[i-1][j] : 0;
        long long ans = 0;
        for (int i = 1; i <= n; i++) {
            debug("Line %d:\n", i);
            sta[sta[0] = 1] = 0;
            sum[0] = 0;
            for (int j = 1; j <= m; j++) {
                if (!a[i][j]) {
                    sta[sta[0] = 1] = j;
                    sum[j] = 0;
                } else {
                    for (; sta[0] > 1 && a[i][sta[sta[0]]] > a[i][j]; --sta[0]);
                    sum[j] = sum[sta[sta[0]]] + (j - sta[sta[0]]) * a[i][j];
                    ans += sum[j];
                    debug("  [+] %d + (%d-%d) * %d\n", sum[sta[sta[0]]], j, sta[sta[0]], a[i][j]);
                    sta[++sta[0]] = j;
                }
            }
        }
        printf("%lld\n", ans % 998244353);
    }
    return 0;
}