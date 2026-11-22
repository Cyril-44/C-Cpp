#include <bits/stdc++.h>
constexpr int N = 200005;
constexpr int64_t INF = 1e18;
#ifdef CLANGD
#define N 25
#endif
int a[N];
std::vector<int> g[N];
using DP = std::vector<int64_t>;
DP f[N], down[N], pl[N], pr[N];
int64_t sum[N];
int k;
DP operator*(const DP &x, const DP &y) {
    DP res(std::min(int(x.size() + y.size()) - 1, k + 1), INF);
    for (int i = 0; i < (int)x.size() && i <= k; i++)
        for (int j = 0; j < (int)y.size() && i+j <= k; j++)
            res[i+j] = std::min(res[i+j], x[i] + y[j]);
    return res;
}
DP& operator*=(DP &x, const DP &y) { (x*y).swap(x); return x; }
void dfs1(int u, int fa) {
    sum[u] = a[u];
    if (g[u].size() == bool(fa)) { f[u] = {a[u], 0}; return; }
    f[u] = {0};
    for (int v : g[u]) if (v != fa) {
        dfs1(v, u);
        f[u] *= f[v];
        sum[u] += sum[v];
    }
    f[u][0] += sum[u];
}
void dfs2(int u, int fa) {
    f[u][0] -= sum[u];
    f[u] *= down[u];
    std::vector<int> sons;
    sons.reserve(g[u].size());
    for (int v : g[u]) if (v != fa) sons.push_back(v);
    const int ss = sons.size();
    if (!ss) return;
    pl[0] = {0};
    for (int i = 1; i < ss; i++)
        pl[i] = pl[i-1] * f[sons[i-1]];
    pr[ss-1] = {0};
    for (int i = ss-1; i >= 1; i--)
        pr[i-1] = pr[i] * f[sons[i]];
    for (int i = 0; i < ss; i++) {
        if (u == 1 && ss == 1) { down[sons[i]] = {a[u], 0}; continue; }
        down[sons[i]] = down[u] * pl[i] * pr[i];
        down[sons[i]][0] += sum[1] - sum[sons[i]];
    }
    for (int v : sons) dfs2(v, u);
}
int main() {
    int n;
    scanf("%d%d", &n, &k);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    for (int u, v, i = 1; i < n; i++) {
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    int lvs = 0;
    for (int i = 1; i <= n; i++) lvs += g[i].size() == 1;
    if (lvs < k) {
        for (int i = 1; i <= n; i++) printf("0 ");
        putchar('\n');
        return 0;
    }
    if (n <= 1000) {
        for (int i = 1; i <= n; i++) {
            dfs1(i, 0);
            printf("%ld ", f[i][std::min((int)f[i].size()-1, k)]);
        }
    } else {
        dfs1(1, 0); down[1] = {0}; dfs2(1, 0);
        for (int i = 1; i <= n; i++)
            printf("%ld ", f[i][std::min((int)f[i].size()-1, k)]);
        putchar('\n');
    }
}