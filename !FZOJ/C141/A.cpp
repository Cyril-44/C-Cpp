#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <vector>
constexpr int N = 10005;
std::vector<int> g[N];
int dfn[N], low[N], sta[N], ts=0, top=0;
int bl2[N], cnt[N], ind[N];
bool insta[N];
void tarj(int u) {
    dfn[u] = low[u] = ++ts;
    sta[++sta[0]] = u; insta[u] = true;
    for (int v : g[u]) {
        if (!dfn[v]) {
            tarj(v);
            low[u] = std::min(low[u], low[v]);
        }
        else if (insta[v]) {
            low[u] = std::min(low[u], dfn[v]);
        }
    }
    if (dfn[u] == low[u]) {
        bl2[u] = ++top;
        cnt[top] = 1;
        while (sta[sta[0]] != u) {
            ++cnt[top];
            insta[sta[sta[0]]] = false;
            bl2[sta[sta[0]--]] = top;
        }
        insta[u] = false;
        --sta[0]; // pop u
    }
}
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int u, v; m--; ) {
        scanf("%d%d", &u, &v);
        g[v].push_back(u);
    }
    for (int i = 1; i <= n; i++)
        if (!dfn[i]) tarj(i);
    for (int i = 1; i <= n; i++)
        for (int j : g[i])
            if (bl2[i] != bl2[j])
                ++ind[bl2[j]];
    int ans = 0, calccnt = 0;
    for (int i = 1; i <= top; i++)
        if (ind[i] == 0)
            ans = cnt[i], ++calccnt;
    if (calccnt > 1) ans = 0;
    printf("%d\n", ans);
    return 0;
}
/* 7 9
2 1
3 2
1 3
1 4
1 5
1 6
7 5
6 7
5 6 */