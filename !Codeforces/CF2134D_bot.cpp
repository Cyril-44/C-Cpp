#include <bits/stdc++.h>
using namespace std;
constexpr int N = 200005;
int d[N], fa[N], path[N];
vector<int> g[N];
bool vis[N];
int fur;
void bfs(int n, int src) {
    for (int i = 0; i <= n; i++) d[i] = -1, fa[i] = -1;
    queue<int> q;
    d[src] = 0;
    q.push(src);
    fur = src;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        if (d[u] > d[fur]) fur = u;
        for (int v : g[u]) if (d[v] == -1) {
            d[v] = d[u] + 1;
            fa[v] = u;
            q.push(v);
        }
    }
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        int n;
        cin >> n;
        for (int i = 1; i <= n; ++i) {
            g[i].clear();
            vis[i] = false;
        }
        for (int i = 0; i < n - 1; ++i) {
            int u, v;
            cin >> u >> v;
            g[u].push_back(v);
            g[v].push_back(u);
        }
        if (n <= 2) {
            puts("-1");
            continue;
        }
        bfs(n, 1);
        int s = fur;
        bfs(n, s);
        int t = fur;
        int len = 0;
        for (int v = t; v != -1; v = fa[v]) path[len++] = v;
        reverse(path, path + len);
        if (len == n) {
            puts("-1");
            continue;
        }
        for (int i = 0; i < len; ++i) vis[path[i]] = true;
        bool printed = false;
        for (int i = 1; i + 1 < len && !printed; ++i) {
            int b = path[i];
            int a_prev = path[i - 1];
            int c = -1;
            for (int v : g[b]) {
                if (!vis[v]) { c = v; break; }
            }
            if (c != -1) {
                printf("%d %d %d\n", a_prev, b, c);
                printed = true;
            }
        }
        for (int i = 0; i < len; ++i) vis[path[i]] = false;
        if (!printed) puts("-1");
    }
    return 0;
}
