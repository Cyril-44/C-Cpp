#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <numeric>
#include <bitset>
#include <queue>
constexpr int N = 2005;
int n;
char g[N][N];
int indeg[N];

int bl2[N], top=0, cnt[N];
std::bitset<N> tot[N];
namespace Tarjan {
    int dfn[N], low[N], sta[N], tot=0;
    bool insta[N];
    void dfs(int u) {
        dfn[u] = low[u] = ++tot;
        insta[u] = true;
        sta[++sta[0]] = u;
        for (int v = 1; v <= n; ++v) if (g[u][v] ^ '0') {
            if (!dfn[v]) dfs(v);
            if (insta[v]) low[u] = std::min(low[u], low[v]);
        }
        if (dfn[u] == low[u]) {
            ++top;
            do {
                bl2[sta[sta[0]]] = top;
                insta[sta[sta[0]]] = false;
                ++cnt[top];
                ::tot[top].set(sta[sta[0]]);
            } while (sta[sta[0]--] != u);
        }
    }
    inline void scc() {
        for (int i = 1; i <= n; i++)
            if (!dfn[i]) dfs(i);
    }
}

bool ng[N][N];
int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        scanf("%s", g[i] + 1);
    Tarjan::scc();
    
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            if ((g[i][j] ^ '0') && (bl2[i] ^ bl2[j]))
                ng[bl2[i]][bl2[j]] = true;
    for (int i = 1; i <= top; i++)
        for (int j = 1; j <= top; j++)
            if (ng[i][j]) ++indeg[j];
    
    static int que[N];
    int l=1, r=1;
    for (int i = 1; i <= top; i++) {
        if (indeg[i] == 0) que[r++] = i;
    }
    while (l < r) {
        int u = que[l++];
        for (int v = 1; v <= top; v++)
            if (ng[u][v]) {
                if (--indeg[v] == 0) que[r++] = v;
                tot[v] |= tot[u];
            }
    }
    int sum = 0;
    for (int i = 1; i <= top; i++)
        sum += cnt[i] * tot[i].count();
    printf("%d\n", sum);
    return 0;
}
