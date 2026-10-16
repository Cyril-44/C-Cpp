#include <bits/stdc++.h>
constexpr int N = 5005;
int64_t f[N][N], fu0[N], fu1[N], tf[N];
std::vector<int> g[N];
int a[N], t[N], sz[N];
int n;
void dfs(int u, int fa) {
    if (g[u].size() == bool(fa)) {
        sz[u] = 1;
        f[u][0] = a[u], f[u][1] = 0;
    } else {
        for (int v : g[u]) if (v != fa) dfs(v, u);
        memset(fu0, 0x3f, sizeof fu0);
        memset(fu1, 0x3f, sizeof fu1);
        fu0[0] = 0, fu1[0] = a[u];
        for (int v : g[u]) if (v != fa) {
            memset(tf, 0x3f, sizeof tf);
            for (int i = 0; i <= sz[u]; i++)
                for (int j = 0; j <= sz[v]; j++)
                    tf[i+j] = std::min(tf[i+j], fu0[i] + f[v][j]);
            memcpy(fu0, tf, sizeof tf);
            memset(tf, 0x3f, sizeof tf);
            for (int i = 0; i <= sz[u]; i++) {
                tf[i + sz[v] - 1] = std::min(tf[i + sz[v] - 1], fu1[i]);
                for (int j = 0; j <= sz[v]; j++)
                    tf[i+j] = std::min(tf[i+j], fu1[i] + f[v][j]);
   
            }
            memcpy(fu1, tf, sizeof tf);
            sz[u] += sz[v]; 
        }
        for (int i = 0; i <= sz[u]; i++)
            f[u][i] = std::min(fu0[i], fu1[i]);
    }
}
int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
    for (int i = 1; i <= n; i++) scanf("%d", &t[i]);
    for (int u, v, i = 1; i < n; i++) {
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    memset(f, 0x3f, sizeof f);
    dfs(1, 0);
    int64_t ans = 1ll << 60;
    for (int i = 0; i <= sz[1]; i++)
        ans = std::min(ans, f[1][i] + t[i]);
    printf("%lld\n", ans);
}