#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <algorithm>
using Status = unsigned long long;
constexpr int N = 8, M = 50;
struct Node { int a, b, c, d; } all[M];
Status right[M], down[M];
int now[N][N];
int n, m;
void dfs2(int, Status); void dfs(int,int,Status);
void dfs1(int i, Status mask) {
    if (i > n) return dfs2(2, mask);
    Status tmp = down[all[now[i-1][1]].c] & mask;
    while (tmp) {
        int id = __builtin_ctz(tmp); tmp ^= 1ull << id;
        now[i][1] = id;
        dfs1(i + 1, mask ^ (1ull << id));
    }
}
void dfs2(int j, Status mask) {
    if (j > m) return dfs(2, 2, mask);
    Status tmp = right[all[now[1][j-1]].b] & mask;
    while (tmp) {
        int id = __builtin_ctz(tmp); tmp ^= 1ull << id;
        now[1][j] = id;
        dfs2(j + 1, mask ^ (1ull << id));
    }
}
void dfs(int i, int j, Status mask) {
    if (i > n) {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++)
                printf("%d ", now[i][j] + 1);
            putchar('\n');
        }
        fclose(stdout);
        exit(0);
    }
    Status tmp = right[all[now[i][j-1]].b] & down[all[now[i-1][j]].c] & mask;
    while (tmp) {
        int id = __builtin_ctz(tmp); tmp ^= 1ull << id;
        now[i][j] = id;
        dfs(i + (j==m), (j==m) ? 2 : j+1, mask ^ (1ull << id));
    }
}
int main() {
#ifdef ONLINE_JUDGE
	freopen("puzzle.in", "r", stdin);
	freopen("puzzle.out", "w", stdout);
#endif
    scanf("%d%d", &n, &m);
    for (int i = 0; i < n*m; i++) {
        scanf("%d%d%d%d", &all[i].a, &all[i].b, &all[i].c, &all[i].d);
        right[all[i].d] |= 1ull << i;
        down[all[i].a] |= 1ull << i;
    }
    for (int i = 0; i < n*m; i++) {
        now[1][1] = i;
        dfs1(2, (~0ull) ^ (1ull << i));
    }
    puts("-1");
    return 0;
}
