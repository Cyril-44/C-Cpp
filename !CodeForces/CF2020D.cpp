#include <stdio.h>
#include <string.h>
const int D = 15, N = 200005;
int f[N<<1][D];
int main() {
    freopen("test.txt", "w", stderr);
    int t, n, m, a, k, d;
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d", &n, &m);
        memset(f, 0, sizeof f);
        while (m--) {
            scanf("%d%d%d", &a, &d, &k);
            --f[a + 1][d], ++f[a + d][d];
            ++f[a + k * d + 1][d], --f[a + (k+1) * d][d];
        for (int j = 1; j <= 10; j++) {
            for (int i = 1; i <= n; i++)
                fprintf(stderr, "%d ", f[i][j]);
            fputc('\n', stderr);
        }
        fprintf(stderr, "------------------------\n");
        }
        for (int j = 1; j <= 10; j++)
            for (int i = j; i <= n; i++)
                f[i][j] += f[i-j][j];
        f[1][0] = 1;
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= 10; j++)
                f[i][0] += f[i][j];
        for (int i = 1; i <= n; i++)
            f[i][0] += f[i-1][0];
        for (int i = 1; i <= n; i++)
            f[i][0] += f[i-1][0];
        printf("%d\n", f[n][0]);
    }
    return 0;
}