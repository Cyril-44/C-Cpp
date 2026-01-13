#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
template <class T> void in(T &x) {
    char c = getchar();
    T f = 1;
    x = 0;
    while (c < '0' || c > '9') {
        if (c == '-') f = -1;
        c = getchar();
    }
    while (c >= '0' && c <= '9') {
        x = x * 10 + c - '0';
        c = getchar();
    }
    x *= f;
}
const int N = 2010;
int n, k, u, v, w;
vector<pii> gr[N];
ll f[3][N][N], g[3][N];
int sz[N];
void dfs(int u, int fa) {
    sz[u] = 1;
    int v, w;
    for (pii x : gr[u]) {
        v = x.first;
        w = x.second;
        if (v == fa) continue;
        dfs(v, u);
        memset(g[0], 0, sizeof(ll) * (sz[u] + sz[v] + 1));
        memset(g[1], 0, sizeof(ll) * (sz[u] + sz[v] + 1));
        memset(g[2], 0, sizeof(ll) * (sz[u] + sz[v] + 1));
        for (int i = 0; i < sz[u]; i++) {
            for (int j = 0; j < sz[v]; j++) {
                g[0][i + j] = max(g[0][i + j], f[0][u][i] + f[0][v][j]);
                g[1][i + j] = max(g[1][i + j], max(f[0][u][i] + f[1][v][j] + w, f[1][u][i] + f[0][v][j]));
                g[2][i + j] = max(g[2][i + j], max(f[1][u][i] + f[1][v][j] + w,
                                                   max(f[2][u][i] + f[0][v][j], f[0][u][i] + f[2][v][j])));
                g[0][i + j + 1] = max(g[0][i + j + 1], f[0][u][i] + f[2][v][j] + w);
                g[1][i + j + 1] = max(g[1][i + j + 1], f[1][u][i] + f[2][v][j] + w);
                g[2][i + j + 1] = max(g[2][i + j + 1], f[2][u][i] + f[2][v][j] + w);
            }
        }
        sz[u] += sz[v];
        for (int i = 1; i <= sz[u]; i++) {
            g[0][i] = max(g[0][i], g[0][i - 1]);
            g[1][i] = max(g[1][i], g[1][i - 1]);
            g[2][i] = max(g[2][i], g[2][i - 1]);
        }
        for (int i = 0; i <= sz[u]; i++) {
            f[0][u][i] = g[0][i];
            f[1][u][i] = max(g[0][i], g[1][i]);
            f[2][u][i] = max(g[1][i], g[2][i]);
        }
    }
}
int main() {
    in(n);
    in(k);
    for (int i = 1; i < n; i++) {
        in(u);
        in(v);
        in(w);
        gr[u].emplace_back(v, w);
        gr[v].emplace_back(u, w);
    }
    dfs(1, 0);
    for (int i = 0; i <= k; i++) printf("%lld ", f[2][1][i]);
}