#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <vector>
constexpr int N = 105;
std::vector<int> g[N];
int n;
int bl2[N], top=0, cnt[N];
int ind[N], outd[N];
namespace Tarjan {
    int dfn[N], low[N], sta[N], tot=0;
    bool insta[N];
    void dfs(int u) {
        dfn[u] = low[u] = ++tot;
        insta[u] = true;
        sta[++sta[0]] = u;
        for (int v : g[u]) {
            if (!dfn[v]) dfs(v);
            if (insta[v]) low[u] = std::min(low[u], low[v]);
        }
        if (dfn[u] == low[u]) {
            ++top;
            do {
                bl2[sta[sta[0]]] = top;
                insta[sta[sta[0]]] = false;
                ++cnt[top];
            } while (sta[sta[0]--] != u);
        }
    }
    inline void scc() {
        for (int i = 1; i <= n; i++)
            if (!dfn[i]) dfs(i);
    }
}
int main() {
    scanf("%d", &n);
    for (int u = 1; u <= n; u++)
        for (int v;;) {
            scanf("%d", &v);
            if (!v) break;
            g[u].push_back(v);
        }
    Tarjan::scc();
    if (top == 1) return puts("1\n0"), 0;
    for (int i = 1; i <= n; i++)
        for (int j : g[i])
            if (bl2[i] != bl2[j])
                ++ind[bl2[j]], ++outd[bl2[i]];
    int ans1 = 0, ans2 = 0;
    for (int i = 1; i <= top; i++) {
        if (!ind[i]) ++ans1;
        if (!outd[i]) ++ans2;
    }
    printf("%d\n%d\n", ans1, std::max(ans1, ans2));
    return 0;
}

