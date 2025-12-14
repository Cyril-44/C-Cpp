#include <stdio.h>
#include <string.h>
const int N = 200005;
int p[N][2];
bool f[N][2];
int main() {
    int t, n, x, y;
    long long cnt;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        memset(f, 0, (n+1) * 2 * sizeof (bool));
        cnt = 0;
        for (int i = 0; i < n; i++) {
            scanf("%d%d", &x, &y);
            f[x][y] = true;
            if (f[x][y^1]) cnt += n - 2;
        }
        for (int i = 1; i < n; i++) { // 等腰直角三角形
            cnt += f[i-1][0] * f[i][1] * f[i+1][0];
            cnt += f[i-1][1] * f[i][0] * f[i+1][1];
        }
        printf("%lld\n", cnt);
    }
    return 0;
}