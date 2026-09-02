#include <bits/stdc++.h>
constexpr int N = 54;
int a[N][N], b[N][N];
int n, p;
bool judge() {
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            if (a[i][j] != b[i][j]) return false;
    return true;
}
int op[N][N];
bool dfs(int x, int y) {
    if (x > n) return judge();
    if (y > n) return dfs(x+1, 1);
    for (op[x][y] = 0; op[x][y] < p; op[x][y]++) {
        if (dfs(x, y+1)) return true;
        for (int i = 1; i <= n; i++) ++a[x][i] %= p;
        for (int j = 1; j <= n; j++) if (j != x) ++a[j][y] %= p;
    }
    return false;
}
int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d", &n, &p);
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++)
                scanf("%d", &a[i][j]);
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++)
                scanf("%d", &b[i][j]);
        if (n > 3) continue;
        if (dfs(1, 1)) {
            puts("Yes");
            printf("%d\n", n*n);
            for (int i = 1; i <= n; i++)
                for (int j = 1; j <= n; j++)
                    printf("%d %d %d\n", i, j, op[i][j]);
        }
        else puts("No");
    }
    return 0;
}