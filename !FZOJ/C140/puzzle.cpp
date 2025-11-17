#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <algorithm>
using Status = unsigned long long;
constexpr int N = 8, M = 50;
struct Node { int a, b, c, d; } all[M];
Status right[M], down[M], rd[M << 6];
int now[N][N];
int n, m;
#define Upd(x, y) ((x) ^= 1ULL << (y))
void dfs2(int); void dfs(int,int);
void dfs1(int i) {
    if (i > n) dfs2(2);
    Status &current = down[now[i-1][1]], tmp = current;
    while (tmp) {
        int id = __builtin_ctz(tmp);
        Upd(current, id), Upd(tmp, id);
        now[i][1] = id;
        dfs1(i + 1);
        Upd(current, id);
    }
}
void dfs2(int j) {
    if (j > m) dfs(2, 2);
    Status &current = right[now[1][j-1]], tmp = current;
    while (tmp) {
        int id = __builtin_ctz(tmp);
        Upd(current, id), Upd(tmp, id);
        now[1][j] = id;
        dfs2(j + 1);
        Upd(current, id);
    }
}
void dfs(int i, int j) {
    if (i > n) {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++)
                printf("%d ", now[i][j]);
            putchar('\n');
        }
        fclose(stdout);
        exit(0);
    }
}
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int i = 0; i < n*m; i++) {
        scanf("%d%d%d%d", &all[i].a, &all[i].b, &all[i].c, &all[i].d);
        right[all[i].d] |= 1 << i;
        down[all[i].c] |= 1 << i;
        rd[all[i].d << 6 | all[i].c] |= 1 << i;
    }

    return 0;
}
