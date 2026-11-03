#include <bits/stdc++.h>
constexpr int N = 1000005;
#ifdef CLANGD
#define N 15
#endif
std::vector<std::pair<int,int>> g[N];
int fr[N], to[N], h[N];
int cur[N], ops[N], path[N][2], top;
bool vis[N];
void dfs(int u) {
    for (int &i = cur[u]; i < (int)g[u].size(); i++) {
        auto [v, id] = g[u][i];
        if (vis[id]) continue;
        vis[id] = true; dfs(v);
        path[++top][0] = v, path[top][1] = id;
    }
}
int main() {
    freopen("machine.in", "r", stdin);
    freopen("machine.out", "w", stdout);
    int T, n;
    scanf("%d", &T);
    while (T--) {
        scanf("%d", &n);
        for (int i = 1; i <= n; i++) g[i].clear();
        memset(h, 0, sizeof(int) * (n+1));
        for (int i = 1; i <= n; i++)
            scanf("%d", &fr[i]), ++h[fr[i]];
        for (int i = 1; i <= n; i++) {
            scanf("%d", &to[i]), ++h[to[i]];
            if (fr[i] != to[i]) {
                g[fr[i]].emplace_back(to[i], i);
                g[to[i]].emplace_back(fr[i], i);
            }
        }
        bool nosol = false;
        for (int i = 1; i <= n; i++)
            if (h[i] & 1) { nosol = true; break; }
        if (nosol) { puts("-1"); continue; }
        
        memset(cur, 0, sizeof(int) * (n+1));
        memset(vis, 0, n+1);
        ops[0] = 0;
        for (int i = 1; i <= n; i++) {
            top = 0; dfs(i);
            for (int j = 1; j <= top; j++)
                if (to[path[j][1]] != path[j][0])
                    ops[++ops[0]] = path[j][1];
        }

        printf("%d\n", ops[0]);
        for (int i = 1; i <= ops[0]; i++) printf("%d ", ops[i]);
        putchar('\n');
    }
}