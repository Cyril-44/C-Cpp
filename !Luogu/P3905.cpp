#include <cstdio>
#include <cstring>
#include <algorithm>
const int N = 105;
int g[N][N], f[N][N];
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    memset(f, 0x3f, sizeof f);
    for (int i = 1; i <= n; i++) f[i][i] = 0;
    for (int i, j, k; m--; ) {
        scanf("%d%d%d", &i, &j, &k);
        g[i][j] = g[j][i] = k;
        f[i][j] = f[j][i] = 0;
    }
    scanf("%d", &m);
    for (int i, j; m--; ) {
        scanf("%d%d", &i, &j);
        f[i][j] = g[i][j];
        f[j][i] = g[j][i];
    }
    for (int k = 1; k <= n; k++)
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++)
                f[i][j] = std::min(f[i][j], f[i][k] + f[k][j]);
    int a, b;
    scanf("%d%d", &a, &b);
    printf("%d\n", f[a][b]);
    return 0;
}