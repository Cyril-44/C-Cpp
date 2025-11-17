#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <vector>
constexpr int N = 100005;
std::vector<int> g[N];
int bl2[N], top;
namespace Tarjan {
    unsigned dfn[N], low[N], ts;
    int sta[N]; bool insta[N];
    void dfs (int u) {
        dfn[u] = low[u] = ++ts;
        sta[++sta[0]] = u;
        insta[u] = true;
        for (int v : g[u]) {
            if (!dfn[v]) dfs(v), low[u] = std::min(low[u], low[v]);
            else if (insta[v]) low[u] = std::min(low[u], dfn[v]);
        }
        if (dfn[u] == low[u]) {
            ++top;
            do {
                int now = sta[sta[0]];
                bl2[now] = top;
                insta[now] = false;
            } while (sta[sta[0]--] != u);
        }
    }
    inline void tarj(int n) {
        for (int i = 1; i <= n; i++)
            if (!dfn[i]) dfs(i);
    }
}
bool hasin[N];
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int u, v; m--;) {
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
    }
    Tarjan::tarj(n);
    for (int i = 1; i <= n; i++)
        for (int j : g[i])
            if (bl2[i] ^ bl2[j])
                hasin[bl2[j]] = true;
    int ans = 0;
    for (int i = 1; i <= top; i++)
        if (!hasin[i]) ++ans;
    printf("%d\n", ans);
    return 0;
}