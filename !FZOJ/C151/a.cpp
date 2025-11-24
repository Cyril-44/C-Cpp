#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
template <typename T> void in(T &x) {
    x = 0;
    char c = getchar();
    int f = 1;
    while (c < '0' || c > '9') {
        if (c == '-')
            f = -1;
        c = getchar();
    }
    while (c >= '0' && c <= '9') {
        x = x * 10 + c - '0';
        c = getchar();
    }
    x *= f;
}
typedef pair<int, int> pii;
const int N = 300010;
vector<int> g[N];
int val[N], cnt;
bool notroot[N];
struct DSU {
    int fa[N];
    DSU() {
        for (int i = 0; i < N; i++)
            fa[i] = i;
    }
    int find(int x) {
        if (x == fa[x])
            return x;
        return fa[x] = find(fa[x]);
    }
    int merge(int x, int y) {
        x = find(x);
        y = find(y);
        if (x == y)
            return -1;
        fa[x] = ++cnt;
        fa[y] = cnt;
        notroot[x] = notroot[y] = 1;
        g[cnt].push_back(x);
        g[cnt].push_back(y);
        return cnt;
    }
} dsu;
struct Edge {
    int u, v, w;
    bool operator<(const Edge &a) { return w < a.w; }
} e[N];
int fa[N][20], dep[N];
void dfs(int u) {
    for (int i = 1; i < 20; i++)
        fa[u][i] = fa[fa[u][i - 1]][i - 1];
    for (int v : g[u]) {
        dep[v] = dep[u] + 1;
        fa[v][0] = u;
        dfs(v);
    }
}
int lca(int u, int v) {
    if (dep[u] < dep[v])
        swap(u, v);
    for (int i = 19; i >= 0; i--)
        if ((dep[u] - dep[v]) & (1 << i))
            u = fa[u][i];
    if (u == v)
        return u;
    for (int i = 19; i >= 0; i--)
        if (fa[u][i] != fa[v][i])
            u = fa[u][i], v = fa[v][i];
    return fa[u][0];
}
int n, m, q, u, v;
int main() {
    in(n);
    in(m);
    cnt = n;
    for (int i = 1; i <= m; i++) {
        in(e[i].u);
        in(e[i].v);
        in(e[i].w);
    }
    sort(e + 1, e + m + 1);
    for (int i = 1; i <= m; i++) {
        if (dsu.merge(e[i].u, e[i].v) != -1)
            val[cnt] = e[i].w;
    }
    for (int i = 1; i <= cnt; i++)
        if (!notroot[i])
            dfs(i);
    in(q);
    while (q--) {
        in(u);
        in(v);
        if (dsu.find(u) != dsu.find(v))
            printf("impossible\n");
        else
            printf("%d\n", val[lca(u, v)]);
    }
    return 0;
}