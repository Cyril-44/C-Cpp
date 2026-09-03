#include <bits/stdc++.h>
#ifdef CLANGD
constexpr int N = 45;
#else
constexpr int N = 400000;
#endif
std::vector<int> sons[N];
int d[N];
int64_t f[N][2], g[N][2], h[N][2][2], s[N][2];
inline void umax(int64_t &x, int64_t y) { (x < y) && (x = y); }
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        scanf("%d", &d[i]);
    for (int i = 2, fa; i <= n; i++) {
        scanf("%d", &fa);
        sons[fa].push_back(i);
    }
    for (int u = n; u >= 1; u--) {
        int64_t sum[2]{};
        g[u][0] = -d[u], g[u][1] = d[u]; 
        for (int v : sons[u]) {
            int64_t hmx = std::max({h[v][0][0], h[v][0][1], h[v][1][0], h[v][1][1]});
            sum[0] += s[v][0] = std::max({f[v][0], g[v][0], g[v][1], hmx});
            sum[1] += s[v][1] = std::max({f[v][1], g[v][0], g[v][1], hmx});
            g[u][0] += std::max(f[v][1], hmx);
            g[u][1] += std::max(f[v][0], hmx);
        }
        f[u][0] = -d[u] + std::max(sum[0], sum[1]);
        f[u][1] = +d[u] + std::max(sum[0], sum[1]);
        memset(h[u], -0x3f, sizeof(int64_t) * 2 * 2);
        for (int v : sons[u]) {
            int64_t v0 = sum[0] - s[v][0], v1 = sum[1] - s[v][1];
            umax(h[u][0][0], v0 + std::max({g[v][1], h[v][0][0], h[v][1][0]}) - d[u]);
            umax(h[u][0][1], v1 + std::max({g[v][1], h[v][0][0], h[v][1][0]}) - d[u]);
            umax(h[u][1][0], v0 + std::max({g[v][0], h[v][0][1], h[v][1][1]}) + d[u]);
            umax(h[u][1][1], v1 + std::max({g[v][0], h[v][0][1], h[v][1][1]}) + d[u]);
        }
    }
    printf("%lld\n", std::max({g[1][0], g[1][1], h[1][0][0], h[1][0][1], h[1][1][0], h[1][1][1]}));
    return 0;
}
