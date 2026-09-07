#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
constexpr int N = 100004;
int a[N], f[N], deg[N], que[N];
std::vector<int> g[N];
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
    for (int i = 1, u, v; i <= m; i++) {
        scanf("%d%d", &u, &v);
        g[v].push_back(u), ++deg[u];
    }
    memset(f, 0x3f, sizeof f);
    f[n] = a[n] / 2 + 1;
    int hd=0, tl=0;
    que[tl++] = n;
    int ans = f[0];
    while (hd < tl) {
        int u = que[hd++];
        if (g[u].empty()) ans = std::min(ans, f[u]);
        for (int v : g[u]) {
            f[v] = std::min(f[v], f[u] >= a[v] ? std::max(f[u] - a[v], a[v]) : (f[u] + a[v] + 1) / 2);
            if (!--deg[v]) que[tl++] = v;
        }
    }
    printf("%d\n", ans);
}