#include <bits/stdc++.h>
constexpr int N = 2005;
std::vector<int> g[N];
int cnt[N], dep[N], fa[N];
void dfs(int u, int fa) {
    ::fa[u] = fa, dep[u] = dep[fa] + 1;
    for (int v : g[u]) if (v != fa)
        dfs(v, u);
}
int main() {
    freopen("migrate.in", "r", stdin);
    freopen("migrate.out", "w", stdout);
    int n;
    scanf("%d", &n);
    for (int i = 1; i < n; i++) {
        int u, v;
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    dfs(1, 0); int u = 0;
    for (int i = 1; i <= n; i++) if (dep[i] > dep[u]) u = i;
    dfs(u, 0); int v = 0;
    for (int i = 1; i <= n; i++) if (dep[i] > dep[v]) v = i;
    std::vector<int> ans; ans.reserve(10000);
    for (int i = v, pre = 0; i; pre = i, i = fa[i]) {
        for (int j : g[i]) if (j != fa[i] && j != pre) {
            for (int k : g[j]) if (k != i)
                if (g[k].size() != 1) { puts("NO"); return 0; }
            ans.push_back(i);
            ans.push_back(j);
        }
        ans.push_back(i);
    }
    if (ans.empty()) ans = {1};
    printf("YES\n%zu\n", ans.size() * 2);
    for (int x : ans) printf("%d ", x);
    std::reverse(ans.begin(), ans.end());
    for (int x : ans) printf("%d ", x);
    putchar('\n');
}