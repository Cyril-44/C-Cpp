#include <cstdio>
#include <cstring>
#include <algorithm>
constexpr int N = 504;
int a[N][N], n;
int64_t sum[N][N], mx[N*4];
int mxid[N*4][4];
template<typename T> void umax(T &x, T y) { if (x < y) x = y; }
int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++) {
            scanf("%d", &a[i][j]);
            sum[i][j] = sum[i-1][j] + sum[i][j-1] - sum[i-1][j-1] + a[i][j];
        }
    memset(mx, -0x3f, sizeof mx);
    for (int lx = 1; lx <= n; lx++)
        for (int ly = 1; ly <= n; ly++)
            for (int rx = lx; rx <= n; rx++)
                for (int ry = ly; ry <= n; ry++) {
                    auto curr = sum[rx][ry] - sum[rx][ly-1] - sum[lx-1][ry] + sum[lx-1][ly-1];
                    int c = 2*(rx-lx+1)+2*(ry-ly+1);
                    if (curr > mx[c]) mx[c] = curr, mxid[c][0] = lx, mxid[c][1] = ly, mxid[c][2] = rx, mxid[c][3] = ry;
                }
    double ans = -1e100; int id[4];
    for (int c = 4; c <= n*4; c++) {
        auto curr = mx[c] * 1. / c;
        if (curr > ans) ans = curr, memcpy(id, mxid[c], sizeof id);
    }
    printf("%.10lf\n%d %d\n%d %d\n", ans, id[1], id[0], id[3], id[2]);
    return 0;
}