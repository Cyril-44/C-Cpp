#include <bits/stdc++.h>
constexpr int N = 9;
int a[N], f[N][N];
bool g[N][N][N];
int n, m, k;
int ans;
inline void check() {
    for (int i = 1; i <= n; i++)
        for (int j = i; j <= n; j++) {
            bool cnt[10]{};
            for (int k = i; k <= j; k++) cnt[a[k]] = true;
            int all = 0;
            for (int k = 1; k <= m; k++) all += cnt[k];
            f[i][j] = all;
        }
    memset(g, 0, sizeof g);
    for (int cl = 1; cl <= m; cl++) g[cl][0][0] = true;
    for (int i = 1; i <= n; i++)
        for (int j = 0; j < i; j++)
            for (int k = 0; k < i; k++)
                g[f[j+1][i]][i][k+1] |= g[f[j+1][i]][j][k];
    for (int i = 1; i <= m; i++)
        if (g[i][n][k]) {++ans; return;}
}
void dfs(int p) {
    if (p > n) return check();
    if (a[p] == 0)
        for (a[p] = 1; a[p] <= m; a[p]++)   
            dfs(p+1);
    else dfs(p+1);
}
int main() {
    scanf("%d%d%d", &n, &m, &k);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    dfs(1);
    printf("%d\n", ans);
    return 0;
}