#include <bits/stdc++.h>
constexpr int N = 2004;
int f[N], a[N][N];
bool vis[N];
int main(int argc, char **argv) {
    FILE *in = fopen(argv[1], "r"), *out = fopen(argv[2], "r");
    int tid, T;
    fscanf(in, "%d%d", &tid, &T);
    for (int tc = 1; tc <= T; tc++) {
        int n;
        fscanf(in, "%d", &n);
        for (int i = 1; i <= n; i++)
            fscanf(in, "%d", &f[i]);
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++)
                fscanf(out, "%d", &a[i][j]);
        for (int i = 1; i <= n; i++) {
            memset(vis, 0, sizeof(bool) * (n+1));
            for (int j = 1; j <= n; j++) vis[a[i][j]] = true;
            for (int j = 0; j < f[i]; j++) if (!vis[j]) {
                printf("Wrong answer on tc %d line %d!\n", tc, i);
                return 1;
            }
            if (vis[f[i]]) {
                printf("Wrong answer on tc %d line %d!!\n", tc, i);
                return 2;
            }
        }
        for (int j = 1; j <= n; j++) {
            memset(vis, 0, sizeof(bool) * (n+1));
            for (int i = 1; i <= n; i++) vis[a[i][j]] = true;
            for (int i = 0; i < f[j]; i++) if (!vis[i]) {
                printf("Wrong answer on tc %d col %d!\n", tc, j);
                return 1;
            }
            if (vis[f[j]]) {
                printf("Wrong answer on tc %d col %d!!\n", tc, j);
                return 2;
            }
        }
    }
    return 0;
}