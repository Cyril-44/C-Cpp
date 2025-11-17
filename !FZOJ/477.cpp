#include <stdio.h>
#include <stdlib.h>
using Status = unsigned long long;
constexpr int N = 8, M = 50;
struct Node { int a, b, c, d; } all[M];
Status right[M], down[M];
int now[N][N];
int n, m;
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
    // fprintf(stderr, "%d %d %llX %llX\n", i, j, mask, tmp);
    while (tmp) {
        int id = __builtin_ctzll(tmp); tmp ^= 1ull << id;
        now[i][j] = id;
        dfs(i + (j==m), (j==m) ? 1 : j+1, mask ^ (1ull << id));
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
    for (int i = 0; i <= n; i++) now[i][0] = n*m;
    for (int j = 1; j <= m; j++) now[0][j] = n*m;
    down[0] = right[0] = ~0ull >> (64 - n*m);
    dfs(1, 1, *down);
    puts("-1");
    return 0;
}