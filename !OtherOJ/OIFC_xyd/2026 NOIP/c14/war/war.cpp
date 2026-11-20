#include <bits/stdc++.h>
#define L first
#define R second
constexpr int N = 501;
using Pii = std::pair<int,int>;
Pii a[N];
std::vector<int> g[N];
int sta[N], sz[N], f[N][N][N], tf[N][N], ans[N];
void dfs(int u) {
    sz[u] = 0;
    f[u][0][0] = 0;
    for (int v : g[u]) {
        dfs(v);
        memset(tf, 0x3f, sizeof tf);
        for (int i = 0; i <= sz[u]; i++)
            for (int j = 0; j <= sz[u]; j++)
                for (int k = 0; k <= sz[v]; k++)
                    for (int l = 0; l <= sz[v]; l++)
                        tf[i+k][j+l] = std::min(tf[i+k][j+l], f[u][i][j] + f[v][k][l]);
        memcpy(f[u], tf, sizeof tf);
        sz[u] += sz[v];
    }
    if (u == 0) return;
    memset(tf, 0x3f, sizeof tf);
    int len = a[u].R - a[u].L + 1;
    for (int i = 0; i <= sz[u]; i++)
        for (int j = 0; j <= sz[u]; j++) {
            if (i <= len) tf[i][j] = std::min(tf[i][j], f[u][i][j] + len - i);
            if (i < len) tf[i+1][j] = std::min(tf[i+1][j], f[u][i][j] + len - i - 1);
            if (j < len) tf[i][j+1] = std::min(tf[i][j+1], f[u][i][j]);
        }
    memcpy(f[u], tf, sizeof tf);
    sz[u]++;
}
int main() {
    freopen("war.in", "r", stdin);
    freopen("war.out", "w", stdout);
    int Tid, T, n;
    scanf("%d%d", &Tid, &T);
    while (T--) {
        scanf("%d", &n);
        for (int i = 0; i <= n; i++) g[i].clear();
        for (int i = 1; i <= n; i++)
            scanf("%d%d", &a[i].L, &a[i].R);
        std::sort(a+1, a+1+n, [](const Pii &x, const Pii &y) { return x.L < y.L || x.L == y.L && x.R > y.R; });
        a[0] = {1, n};
        int top = 0;
        for (int i = 1; i <= n; i++) {
            while (a[sta[top]].R < a[i].L) --top;
            g[sta[top]].push_back(i);
            sta[++top] = i;
        }
        dfs(0);
        memset(ans, 0x3f, sizeof ans);
        for (int i = 0; i <= n; i++)
            for (int j = 0; i+j <= n; j++)
                ans[i+j] = std::min(ans[i+j], f[0][i][j]);
        for (int i = 1; i <= n; i++) ans[i] = std::min(ans[i], ans[i-1]);
        for (int i = 0; i <= n; i++) printf("%d ", ans[i]);
        putchar('\n');
    }
}