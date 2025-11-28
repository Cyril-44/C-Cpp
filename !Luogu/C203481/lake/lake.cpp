#include <stdio.h>
#include <string.h>
#include <algorithm>
using std::min;
const int N = 3005, D[][2] = {{0,1}, {1,0}, {0,-1}, {-1,0}};
int l[N][N], f[N][N];
struct Lake {
    int a1, b1, a2, b2;
} a[N];
struct Tree {
    int t, x, y;
    bool needToBeSaved;
} b[N];
int main() {
    int n, m, q, r, k, a1, b1, a2, b2;
    scanf("%d%d%d%d%d", &n, &m, &q, &r, &k);
    for (int i = 1; i <= q; i++) {
        scanf("%d%d%d%d", &a[i].a1, &a[i].b1, &a[i].a2, &a[i].b2);
        for (int j = a[i].a1; j <= a[i].a2; j++) {
            l[j][a[i].b1] = i;
            l[j][a[i].b2] = i;
        }
        for (int j = a[i].b1; j <= a[i].b2; j++) {
            l[a[i].a1][j] = i;
            l[a[i].a2][j] = i;
        }
    }
    memset(f, 0x7f, sizeof f);
    auto upd = [=] (const int &x, const int &y, const int &t) -> bool {
        bool flg = false;
        for (int d = 0; d < 3; d++) {   // 到tx,ty的时候是t+1了
            int tx = x + D[d][0], ty = y + D[d][1];
            if (tx < 1 || tx > n || ty < 1 || ty > m) continue;
            if (l[tx][ty]) {
                flg = true;
                switch(d) {
                case 0: // 遇到左边界
                case 2: // 遇到右边界
                    for (int j = a[l[tx][ty]].b1; j <= a[l[tx][ty]].b2; j++) {
                        f[tx][j] = min(f[tx][j], t + 1 + abs(j - ty));
                    }
                    break;
                case 1: // 遇到下边界
                case 3: // 遇到上边界
                    for (int i = a[l[tx][ty]].a1; i <= a[l[tx][ty]].a2; i++)
                        f[i][ty] = min(f[i][ty], t + 1 + abs(i - tx));
                    break;
                }
            }
        }
        return flg;
    };
    for (int i = 0; i < r; i++) {
        scanf("%d%d%d", &b[i].t, &b[i].x, &b[i].y);
        bool flg = false;
        if (upd(b[i].x, b[i].y, b[i].t)) flg = true;
        for (int d = 0; d < 3; d++) {
            int tx = b[i].x + D[d][0], ty = b[i].y + D[d][1];
            if (tx < 1 || tx > n || ty < 1 || ty > m) continue;
            if (!l[tx][ty] && upd(tx, ty, b[i].t + 1))
                flg = true;
        }
        if (!flg) b[i].needToBeSaved = true;
    }
    for (int i = 0; i < r; i++) {
        if (b[i].needToBeSaved) {
            for (int d = 0; d < 4; d++) {
                int tx = b[i].x + D[d][0], ty = b[i].y + D[d][1];
                if (tx < 1 || tx > n || ty < 1 || ty > m) continue;
                if (f[tx][ty] - b[i].t <= k) {
                    f[b[i].x][b[i].y] = b[i].t;
                    break;
                }
            }
        }
    }
    int ans = 0;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            if (f[i][j] < 0x7f7f7f7f) ++ans;
    printf("%d\n", ans);
    return 0;
}