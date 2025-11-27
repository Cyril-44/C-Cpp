#include <stdio.h>
#include <set>
const int N = 205;
int d[N], tmp[N];
std::set<int> g[N];
void dfs(int u) {
    for (const int& i : g[u]) {
        for (const int& j : g[i])
            if ((j^u) && g[j].count(u)) {
                --d[i], --d[j], --d[u];
            }
    }
    tmp[0] = 0;
    for (const int& i : g[u])
        tmp[++tmp[0]] = i, g[i].erase(u);
    g[u].clear();
    for (int i = 1; i <= tmp[0]; i++)
        dfs(tmp[i]);
}
int main() {
    int t, n, m, u, v;
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d", &n, &m);
        for (int i = 1; i <= n; i++)
            g[i].clear();
        while (m--) {
            scanf("%d%d", &u, &v);
            g[u].insert(v);
            g[v].insert(u);
        }
        bool flg = true;
        for (int i = 1; flg && i <= n; i++) {
            d[i] = g[i].size();
            if (g[i].size() < 2)
                flg = false;
        }
        if (flg) {
            dfs(1);
            for (int i = 1; flg && i <= n; i++)
                if (g[i].size()) flg = false;
        }
        puts(flg ? "Yes" : "No");
    }
    return 0;
}