#include <stdio.h>
#include <string.h>
#include <queue>
#include <bits/stl_pair.h>
const int N = 200005;
const long long INF = 0x3f3f3f3f3f3f3f3fLL;
bool h[N];
struct Edge {
    int to, nxt, w;
} e[N << 1];
int head[N];
inline void addedg(const int &fr, const int &to, const int &t) {
    e[++head[0]] = {to, head[fr], t};
    head[fr] = head[0];
}
long long f[N][2], g[N][2];
typedef std::pair<int, int> PII;
inline void dij(int u, const int &n, long long f[][2]) {
    int vis[n+1] = {};
    memset(f, 0x3f, (n+1) * 2 * sizeof(long long));
    vis[u] = 0;
    std::priority_queue<PII, std::vector<PII>, std::greater<PII>> q;
    f[u][0] = 0;
    q.push(PII(f[u][0], u));
    while (!q.empty()) {
        u = q.top().second, q.pop();
        if (vis[u] >= 2) continue;
        ++vis[u];
        for (int i = head[u], v; i; i = e[i].nxt) {
            if (vis[v = e[i].to] < 2) {
                if (f[v][0] > f[u][0] + e[i].w) {
                    f[v][0] = f[u][0] + e[i].w;
                    q.push(PII(f[v][0], v));
                }
                if ((h[u] && f[v][1] > f[u][0] + (e[i].w>>1)) || f[v][1] > f[u][1] + (e[i].w>>1)) {
                    if (!h[u] || f[u][1] + (e[i].w>>1) < f[u][0] + (e[i].w>>1))
                        f[v][1] = f[u][1] + (e[i].w>>1);
                    else f[v][1] = f[u][0] + (e[i].w>>1);
                    q.push(PII(f[v][1], v));
                }
            }
        }
    }
}
int main() {
    int t, n, m, hc, hx, u, v, w;
    long long ans;
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d%d", &n, &m, &hc);
        memset(h, 0, n+1);
        while (hc--) {
            scanf("%d", &hx);
            h[hx] = true;
        }
        memset(head, 0, (n+1) * sizeof(int));
        while (m--) {
            scanf("%d%d%d", &u, &v, &w);
            addedg(u, v, w);
            addedg(v, u, w);
        }
        dij(1, n, f);
        dij(n, n, g);
        ans = INF;
        for (int i = 1; i <= n; i++) {
            ans = std::min(ans, std::max(f[i][0], g[i][0]));
            ans = std::min(ans, std::max(f[i][0], g[i][1]));
            ans = std::min(ans, std::max(f[i][1], g[i][0]));
            ans = std::min(ans, std::max(f[i][1], g[i][1]));
        }
        if (ans ^ INF) printf("%lld\n", ans);
        else puts("-1");
    }
    return 0;
}