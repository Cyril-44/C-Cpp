#include <bits/stdc++.h>
constexpr int N = 1005, Q = 300005;
#ifdef CLANGD
#define N 15
#define Q 35
#endif
#define X first
#define Y second
char s[N][N];
int sum[N][N], ans[Q];
std::pair<int,int> a[Q], b[Q];
std::pair<int,int> que[N*N];
int flg[N][N], n;
int main() {
    int q;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%s", s[i] + 1);
        for (int j = 1; j <= n; j++)
            sum[i][j] = sum[i-1][j] + sum[i][j-1] - sum[i-1][j-1] + (s[i][j] == '#');
    }
    scanf("%d", &q);
    for (int i = 1; i <= q; i++)
        scanf("%d%d%d%d", &a[i].X, &a[i].Y, &b[i].X, &b[i].Y);
    for (int sz = n+1 >> 1; sz > 0; sz--) {
        memset(flg, -1, sizeof flg);
        for (int i = sz; i+sz-1 <= n; i++)
            for (int j = sz; j+sz-1 <= n; j++)
                if (sum[i+sz-1][j+sz-1] - sum[i+sz-1][j-sz] - sum[i-sz][j+sz-1] + sum[i-sz][j-sz] == 0)
                    flg[i][j] = 0;
        int top = 0;
        for (int i = sz; i+sz-1 <= n; i++)
            for (int j = sz; j+sz-1 <= n; j++)
                if (!flg[i][j]) {
                    int hd=0, tl=0;
                    que[tl++] = {i, j};
                    flg[i][j] = ++top;
                    while (hd < tl) {
                        auto [x, y] = que[hd++];
                        for (const auto &[dx, dy] : {std::pair<int,int>{-1,0},{0,1},{1,0},{0,-1}}) {
                            int tx = x + dx, ty = y + dy;
                            if (!flg[tx][ty]) flg[tx][ty] = top, que[tl++] = {tx, ty};
                        }
                    }
                }
        for (int i = 1; i <= q; i++)
            if (!ans[i] && ~flg[a[i].X][a[i].Y] && flg[a[i].X][a[i].Y] == flg[b[i].X][b[i].Y])
                ans[i] = sz * 2 - 1;
    }
    for (int i = 1; i <= q; i++) printf("%d\n", ans[i]);
}