#include <bits/stdc++.h>

using namespace std;

#define int long long

int n, m;
long long all;
int pos[2005], t[2005], l[2005], r[2005], w[2005], p[2005], sum[10005];
// int val[205][205];
bool vis[4005];

struct Edge {
    int v, cap, val;
    int rev;
};
vector<Edge> e[4005];
int iter[4005], S, T, dep[4005];

void add(int x, int y, int z, int c) {
    // printf("addedge: %d %d %d %d\n", x, y, z, c);
    e[x].push_back({y, z, c, (int)e[y].size()});
    e[y].push_back({x, 0, -c, (int)e[x].size() - 1});
}

bool bfs() {
    queue<int> q;
    q.push(S);
    for (int i = 0; i <= 2 * n + 1; i++) dep[i] = -0x7f7f7f7f7f7f7f7fll;
    // memset(dep, -0x3f, sizeof(dep));
    dep[S] = 0;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (auto now : e[u]) {
            int v = now.v;
            if (now.cap && dep[v] < dep[u] + now.val) {
                dep[v] = dep[u] + now.val;
                q.push(v);
            }
        }
    }
    return dep[T] != -0x7f7f7f7f7f7f7f7fll;
}

int dfs(int u, int flow) {
    if (!flow || u == T) return flow;
    int res = 0, d;
    for (int &i = iter[u]; i < e[u].size(); i++) {
        auto &now = e[u][i];
        int v = e[u][i].v;
        if (vis[v]) continue;
        if (dep[v] == dep[u] + now.val && now.cap) {
            // printf("!!: %d %d: %d %d\n", u, v, dep[u], dep[v]);
            vis[v] = 1;
            d = dfs(v, min(flow - res, now.cap));
            vis[v] = 0;
            if (d) {
                res += d;
                now.cap -= d;
                e[v][now.rev].cap += d;
                all += d * now.val;
                if (res == flow) return res;
            } 
        }
    }
    return res;
}

int dinic() {
    all = 0;
    int maxflow = 0;
    while (bfs()) {
        memset(iter, 0, sizeof(iter));
        // printf("FDSFDSFSD\n");
        vis[S] = 1;
        maxflow += dfs(S, 0x3f3f3f3f3f3f3f3fll);
        vis[S] = 0;
    }
    return maxflow;
}

signed main() {
    scanf("%lld%lld", &n, &m);
    S = 0, T = 2 * n + 1;
    for (int i = 1; i <= n; i++) {
        scanf("%lld", &t[i]);
        if (t[i] == 1) {
            scanf("%lld%lld%lld%lld", &l[i], &r[i], &w[i], &p[i]);
        } else {
            scanf("%lld%lld%lld", &l[i], &r[i], &p[i]);
        }
        if (p[i]) pos[p[i]] = i;
    }
    for (int i = 1, j = n; i <= n; i++) {
        if (t[i] == 2 && !p[i]) {
            while (pos[j]) j--;
            pos[j] = i;
            p[i] = j;
        }
    }
    long long ans = 0;
    for (int i = n; i >= 1; i--) {
        int u = pos[i];
        if (!u) {
            // printf("!@@FD: %d\n", i);
            for (int j = 1; j <= n; j++) {
                if (!p[j]) {
                    add(j, i + n, 1, w[j] * (sum[r[j]] - sum[l[j] - 1]));
                    // val[i][j] = now;
                }
            }
            // assert(1 == -1);
        } else if (t[u] == 2) {
            for (int j = l[u]; j <= m; j++) {
                sum[j] += min(r[u], j) - l[u] + 1;
            }
        } else if (t[u] == 1) {
            ans += w[u] * (sum[r[u]] - sum[l[u] - 1]);
        }
    }
    for (int i = 1; i <= n; i++) {
        if (!p[i]) {
            add(S, i, 1, 0);
        }
        if (!pos[i]) {
            add(i + n, T, 1, 0);
        }
    }
    dinic();
    ans += all;
    printf("%lld\n", ans);
    return 0;
}