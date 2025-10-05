#include <cstdio>
#include <cstring>
#include <algorithm>
#include <queue>
constexpr int N = 2005, M = 25005;
struct Edge {
    int to, nxt;
    long double prod;
} e[M];
int head[N];
inline void addedg(const int &fr, const int &to, const long double &w) {
    e[++head[0]] = Edge {to, head[fr], w};
    head[fr] = head[0];
}
inline long double spfa(const int &s, const int &t, const int &n, const long double &v) {
    static long double f[N];
    static bool vis[N];
    static int cnt[N];
    std::queue<int> q;
    q.emplace(s);
    for (int i = 1; i <= n; i++) f[i] = (long double)(1e300);
    f[s] = v; cnt[s] = 1;
    memset(vis, 0, sizeof vis);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        vis[u] = false;
        for (int i = head[u], v; i; i = e[i].nxt) {
            v = e[i].to;
            if (f[v] > f[u] * e[i].prod) {
                f[v] = f[u] * e[i].prod;
                if (!vis[v]) {
                    vis[v] = true;
                    ++cnt[v];
                    if (cnt[v] >= n) return 0;
                    q.push(v);
                }
            }
        }
    }
    return f[t];
}
int main() {
    int n, m, a, b;
    long double v, e;
    scanf("%d%d%Lf%d%d", &n, &m, &v, &a, &b);
    for (int i, j; m--; ) {
        scanf("%d%d%Lf", &i, &j, &e);
        addedg(i, j, e);
    }
    printf("%.6Lf\n", spfa(a, b, n, v));
    return 0;
}