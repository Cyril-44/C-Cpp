#include <bits/stdc++.h>
#define ll long long
#define pr pair<int, int>
#define mk make_pair
#pragma GCC optimize(2)
using namespace std;
template <typename T> void in(T &x) {
    int f = 1;
    char c = getchar();
    while ((c < '0' || c > '9') && c != '-') c = getchar();
    if (c == '-') {
        c = getchar();
        f = -1;
    }
    for (x = 0; c >= '0' && c <= '9'; c = getchar()) x = x * 10 + c - '0';
    x *= f;
}
bool cmn(int &x, int y) {
    return (y < x) && ((x = y) || 1);
}
bool cmx(int &x, int y) {
    return (y > x) && ((x = y) || 1);
}
const int N = 1e5 + 5;
int id, T, n, m, q;
struct edges {
    int to, next;
} edge[2 * N];
int head[N], kk;
int deg[N];
vector<int> g[N];
void insert(int a, int b) {
    edge[++kk] = (edges){b, head[a]}, head[a] = kk;
}
namespace sub1 {
const int N = 305, inf = 1e9 + 7;
int d[N][N], dis[N][N][2];
struct node {
    int u, v, op;
};
void wk() {
    int Q = q;
    queue<node> q;
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            dis[i][j][0] = 0, dis[i][j][1] = inf;
            d[i][j] = deg[j] + 1;
        }
        q.push((node){i, i, 0});
        dis[i][i][0] = 0, d[i][i] = 0;
    }
    while (!q.empty()) {
        node n1 = q.front();
        q.pop();
        int u = n1.u, v = n1.v;
        if (!n1.op) {
            for (int i = head[u]; i; i = edge[i].next) {
                int uu = edge[i].to;
                if (cmn(dis[uu][v][1], dis[u][v][0] + 1)) q.push((node){uu, v, 1});
            }
            for (int uu : g[u])
                if (cmn(dis[uu][v][1], dis[u][v][0] + 1)) q.push((node){uu, v, 1});
            if (cmn(dis[u][v][1], dis[u][v][0] + 1)) q.push((node){u, v, 1});
        } else {
            cmx(dis[u][v][0], dis[u][v][1]);
            if (!(--d[u][v])) q.push((node){u, v, 0});
            for (int i = head[v]; i; i = edge[i].next) {
                int vv = edge[i].to;
                cmx(dis[u][vv][0], dis[u][v][1]);
                if (!(--d[u][vv])) q.push((node){u, vv, 0});
            }
        }
    }
    for (int i = 1, x, y; i <= Q; ++i) in(x), in(y), printf("%d\n", x == y ? 0 : dis[x][y][0]);
}
} // namespace sub1
int dis[N], ans;
void dfs(int u, int fa) {
    dis[u] = dis[fa] + 1;
    for (int i = head[u]; i; i = edge[i].next) {
        int v = edge[i].to;
        if (v == fa) continue;
        dfs(v, u);
    }
}
void dfs2(int u, int fa, int step) {
    if (step >= dis[u]) return;
    ans = max(ans, dis[u]);
    for (int i = head[u]; i; i = edge[i].next) {
        int v = edge[i].to;
        if (v == fa) continue;
        dfs2(v, u, step + 1);
    }
}
int main() {
    in(id), in(T);
    while (T--) {
        in(n), in(m), in(q);
        for (int i = 1; i <= n; ++i) g[i].clear(), head[i] = deg[i] = 0;
        kk = 0;
        for (int i = 1, x, y; i < n; ++i) in(x), in(y), insert(x, y), insert(y, x), deg[x]++, deg[y]++;
        for (int i = 1, x, y; i <= m; ++i) in(x), in(y), g[x].push_back(y), g[y].push_back(x);
        if (id <= 6) {
            sub1::wk();
            continue;
        }
        dis[0] = -1;
        for (int i = 1, x, y; i <= q; ++i) {
            in(x), in(y), ans = 0;
            dfs(x, 0), dfs2(y, 0, 0);
            printf("%d\n", ans);
        }
    }
    return 0;
}