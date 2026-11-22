#include <bits/stdc++.h>
constexpr int N = 105;
#ifdef CLANGD
#define N 25
#endif
int a[N];
std::vector<int> g[N];
int f[N][N], sz[N], sum[N], tf[N];
void dfs1(int u, int fa) {
    sum[u] = a[u];
    if (g[u].size() == bool(fa)) {
        sz[u] = 1;
        f[u][0] = a[u];
        f[u][1] = 0;
        return;
    }
    f[u][0] = 0;
    sz[u] = 0;
    for (int v : g[u]) if (v != fa) {
        dfs1(v, u);
        sum[u] += sum[v];
        memset(tf, 0x3f, sizeof tf);
        for (int i = 0; i <= sz[u]; i++)
            for (int j = 0; j <= sz[v]; j++)
                tf[i+j] = std::min(tf[i+j], f[u][i] + f[v][j]);
        memcpy(f[u], tf, sizeof tf);
        sz[u] += sz[v];
    }
    f[u][0] += sum[u];
}
int main() {
    int n, k;
    scanf("%d%d", &n, &k);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    for (int u, v, i = 1; i < n; i++) {
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    for (int i = 1; i <= n; i++) {
        dfs1(i, 0);
        printf("%d ", f[i][std::min(sz[i], k)]);
    }
    putchar('\n');
}