#include <bits/stdc++.h>
using namespace std;
int n, a[500005], fa[500005], w[500005], sz[500005], hvson[500005], dep[500005], t[5000000][3], ans[500005][3];
int in[500005], out[500005], dfn[500005], idx;
vector<int> g[500005];
void dfs(int x) {
    sz[x] = 1;
    w[x] ^= w[fa[x]];
    dep[x] = dep[fa[x]] + 1;
    in[x] = ++idx;
    dfn[idx] = x;
    for (int y : g[x]) {
        dfs(y);
        if (sz[hvson[x]] < sz[y]) hvson[x] = y;
        sz[x] += sz[y];
    }
    out[x] = idx;
}
void add(int x) {
    t[w[x]][dep[x] % 3] = max(t[w[x]][dep[x] % 3], dep[x]);
}
void query(int x, int rt) {
    int pm = (dep[rt] + dep[x]) % 3, pre = dep[x] - 2 * dep[rt];
    for (int i = 0; i < 3; i++) { ans[rt][(pm + i) % 3] = max(ans[rt][(pm + i) % 3], pre + t[w[x]][i]); }
    for (int k = 0; k < 22; k++) {
        for (int i = 0; i < 3; i++) { ans[rt][(pm + i) % 3] = max(ans[rt][(pm + i) % 3], pre + t[w[x] ^ (1 << k)][i]); }
    }
}
void dfs2(int x, bool ep) {
    for (int y : g[x]) {
        if (y == hvson[x]) continue;
        dfs2(y, 0);
        for (int i = 0; i < 3; i++) { ans[x][i] = max(ans[x][i], ans[y][i]); }
    }
    if (hvson[x]) {
        dfs2(hvson[x], 1);
        for (int i = 0; i < 3; i++) { ans[x][i] = max(ans[x][i], ans[hvson[x]][i]); }
    }
    for (int y : g[x]) {
        if (y == hvson[x]) continue;
        for (int i = in[y]; i <= out[y]; i++) query(dfn[i], x);
        for (int i = in[y]; i <= out[y]; i++) add(dfn[i]);
    }
    query(x, x);
    add(x);
    if (!ep) {
        for (int i = in[x]; i <= out[x]; i++) { t[w[dfn[i]]][dep[dfn[i]] % 3] = -1; }
    }
}
int main() {
    cin >> n;
    for (int i = 1; i <= n; i++) {
        scanf("%d", &a[i]);
        ans[i][1] = ans[i][2] = -1;
    }
    for (int i = 2; i <= n; i++) {
        char ch[2];
        scanf("%d%s", &fa[i], ch);
        w[i] = 1 << (ch[0] - 'a');
        g[fa[i]].push_back(i);
    }
    memset(t, -1, sizeof(t));
    dfs(1);
    dfs2(1, 0);
    for (int i = 1; i <= n; i++) { printf("%d ", ans[i][a[i]]); }
    printf("\n");
    return 0;
}