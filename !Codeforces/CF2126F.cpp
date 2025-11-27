#include <cstdio>
#include <cstring>
#include <map>
// #pragma GCC optimize(2)
const int N = 200005;
int a[N];
struct Edge {
    int to, nxt, w;
} e[N << 1];
int head[N];
inline void addedg(const int &fr, const int &to, const int &w) {
    e[++head[0]] = Edge {to, head[fr], w};
    head[fr] = head[0];
}
std::map<int, long long> c[N];
int fa[N], faw[N];
inline void dfs(const int &u, const int &r) {
    for (int i = head[u]; i; i = e[i].nxt) {
        if (e[i].to ^ r) {
            fa[e[i].to] = u;
            faw[e[i].to] = e[i].w;
            dfs(e[i].to, u);
        }
    }
}
int main() {
    int t, n, q;
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d", &n, &q);
        memset(head, 0, sizeof(int) * (n+1));
        for (int i = 1; i <= n; i++)
            scanf("%d", &a[i]);
        for (int i = 1; i < n; i++) {
            int u, v, c;
            scanf("%d%d%d", &u, &v, &c);
            addedg(u, v, c);
            addedg(v, u, c);
        }
        dfs(1, 0);
        long long ans = 0;
        for (int u = 1; u <= n; u++) {
            c[u].clear();
            for (int i = head[u], v; i; i = e[i].nxt) {
                if ((v = e[i].to) == fa[u]) continue;
                c[u][a[v]] += e[i].w;
                if (a[v] ^ a[u]) ans += e[i].w;
            }
        }
        while (q--) {
            int u, b;
            scanf("%d%d", &u, &b);
            // fa
            if (a[u] != a[fa[u]] && b == a[fa[u]]) ans -= faw[u];
            else if (a[u] == a[fa[u]] && b != a[fa[u]]) ans += faw[u];
            c[fa[u]][a[u]] -= faw[u];
            c[fa[u]][b] += faw[u];
            // sons
            ans -= c[u][b];
            ans += c[u][a[u]];
            // u
            a[u] = b;
            printf("%lld\n", ans);
        }
    }
    return 0;
}