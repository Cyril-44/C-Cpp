#include <stdio.h>
#include <algorithm>
#pragma GCC optimize(3)
using std::min;
using std::max;
const int N = 405, INF = 0x3f3f3f3f;
int g[N][N], f[N][N], lat[N];
bool vis[N], isp[N];
int main() {
    int t, n, m, p, u, v, w, id, si;
    long long mx, red, tot;
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d%d", &n, &m, &p);
        tot = 0;
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                g[i][j] = INF;
                f[i][j] = (i ^ j) ? INF : 0;
            }
            vis[i] = false;
            isp[i] = false;
            lat[i] = INF;
        }
        while (p--) {
            scanf("%d", &si);
            isp[si] = true;
            tot += INF;
        }
        while (m--) {
            scanf("%d%d%d", &u, &v, &w);
            g[u][v] = g[v][u] = w;
            f[u][v] = f[v][u] = w;
        }
        for (int k = 1; k <= n; k++)
            for (int i = 1; i <= n; i++)
                for (int j = 1; j <= n; j++)
                if ((i ^ j) && (i ^ k) && (j ^ k))
                    f[i][j] = min(f[i][j], max(f[i][k], f[k][j]));
        for (int k = 1; k <= n; k++) {
            mx = 0, id = -1;
            for (int i = 1; i <= n; i++) {
                if (vis[i]) continue;
                red = 0;
                for (int j = 1; j <= n; j++)
                    if (isp[j])
                    red += max(lat[j] - f[i][j], 0);
                if (red > mx) mx = red, id = i;
            }
            for (int j = 1; j <= n; j++)
                lat[j] = min(lat[j], f[id][j]);
            vis[id] = true;
            tot -= mx;
            printf("%lld ", tot);
        }
        putchar('\n');
    }
    return 0;
}