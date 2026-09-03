#include <bits/stdc++.h>
constexpr int N = 404;
std::vector<int> g[N];
int64_t f[N][N][N], t[N][N];
int a[N], sz[N], n;
/*
提前钦定每一个点在子树内的贡献次数、整颗树上的贡献次数，转移的时候使限制满足即可。
*/
int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
    for (int i = 2, p; i <= n; i++) {
        scanf("%d", &p);
        g[p].push_back(i);
    }
    memset(f, -0x3f, sizeof f);
    for (int u = n; u >= 1; u--) {
        sz[u] = 1;
        for (int i = 1; i <= n; i++)
            f[u][i][1] = 1ll * a[u] * i;
        for (int v : g[u]) {
            memset(t, -0x3f, sizeof t);
            for (int i = 1; i <= n; i++)
                for (int j = 1; j <= sz[u]; j++) {
                    for (int k = 1; k <= sz[v]; k++)
                        t[i][j+k] = std::max(t[i][j+k], f[u][i][j] + f[v][k][k]);
                    for (int k = 1, _ = std::min(n-i, sz[v]); k <= _; k++)
                        t[i][j] = std::max(t[i][j], f[u][i][j] + f[v][i+k][k]);
                }
            sz[u] += sz[v];
            memcpy(f[u], t, sizeof t);
        }
    }
    int64_t ans = f[0][0][0];
    for (int i = 1; i <= n; i++)
        ans = std::max(ans, f[1][i][i]);
    printf("%lld\n", ans);
    return 0;
}