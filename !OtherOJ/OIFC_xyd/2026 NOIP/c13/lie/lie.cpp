#include <bits/stdc++.h>
constexpr int N = 505;
bool g[N][N], dg[N][N]; char str[N]; bool insta[N];
int dfn[N], low[N], sta[N], belong[N], mx[N], val[N], n;
void tarjan(int u) {
    low[u] = dfn[u] = ++dfn[0];
    sta[++sta[0]] = u, insta[u] = true;
    for (int v = 1; v <= n; v++) if (g[u][v]) {
        if (!dfn[v]) tarjan(v), low[u] = std::min(low[u], low[v]);
        else if (insta[v]) low[u] = std::min(low[u], dfn[v]);
    }
    if (dfn[u] == low[u]) {
        ++belong[0];
        do belong[sta[sta[0]]] = belong[0], insta[sta[sta[0]]] = false;
        while (sta[sta[0]--] != u);
    }
}
int main() {
    freopen("lie.in", "r", stdin);
    freopen("lie.out", "w", stdout);
    int Tid, q, K;
    scanf("%d%d%d%d", &Tid, &n, &q, &K);
    for (int i = 1; i <= n; i++) {
        scanf("%s", str+1);
        for (int j = 1; j <= n; j++)
            g[i][j] = str[j] ^ '0';
    }
    for (int op, x, y; q--; ) {
        scanf("%d%d%d", &op, &x, &y);
        if (op == 1) g[x][y] = true;
        else g[x][y] = g[y][x] = false;
        memset(dfn, 0, sizeof dfn);
        memset(dg, 0, sizeof dg);
        memset(sta, 0, sizeof sta);
        memset(belong, 0, sizeof belong);
        memset(insta, 0, sizeof insta);
        memset(val, 0, sizeof val);
        memset(mx, 0, sizeof mx);
        for (int i = 1; i <= n; i++)
            if (!dfn[i]) tarjan(i);
        for (int i = 1; i <= n; i++)
            mx[belong[i]] = std::max(mx[belong[i]], i);
        for (int i = 1; i <= n; i++)
            val[belong[i]] ^= i * mx[belong[i]];
        int ans = 0;
        for (int i = 1; i <= belong[0]; i++)
            ans += val[i];
        printf("%d\n", ans);
    }
}