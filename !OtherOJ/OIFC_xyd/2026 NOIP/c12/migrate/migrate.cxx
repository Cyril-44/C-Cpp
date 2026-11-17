#include <bits/stdc++.h>
using namespace std;
int n, dis[2005], fa[2005], mxson[2005], ans[4005], k;
vector<int> g[2005];
void dfs(int x, int fth) {
    fa[x] = fth;
    for (int y : g[x]) {
        if (y == fth) continue;
        dis[y] = dis[x] + 1;
        dfs(y, x);
    }
}
bool check(int x, int fth) {
    for (int y : g[x]) {
        if (y != fth && g[y].size() > 1) return 0;
    }
    return 1;
}
void work(int x, int fth) {
    if (!mxson[x]) return;
    ans[++k] = x;
    for (int y : g[x]) {
        if (y == fth || y == mxson[x]) continue;
        if (g[y].size() == 1) continue;
        if (!check(y, x)) {
            puts("NO");
            exit(0);
        }
        ans[++k] = y;
        ans[++k] = x;
    }
    work(mxson[x], x);
}
int main() {
    freopen("migrate.in", "r", stdin);
    freopen("migrate.out", "w", stdout);
    cin >> n;
    if (n == 1) {
        printf("YES\n1\n1\n");
        return 0;
    }
    if (n == 2) {
        printf("YES\n2\n1 1\n");
        return 0;
    }
    for (int i = 1; i < n; i++) {
        int x, y;
        scanf("%d%d", &x, &y);
        g[x].push_back(y);
        g[y].push_back(x);
    }
    dfs(1, 0);
    int x = 0, y = 0;
    for (int i = 1; i <= n; i++) {
        if (dis[x] < dis[i]) x = i;
    }
    fa[x] = 0;
    dis[x] = 0;
    dfs(x, 0);
    for (int i = 1; i <= n; i++) {
        if (dis[y] < dis[i]) y = i;
    }
    for (int i = y; i != x; i = fa[i]) { mxson[fa[i]] = i; }
    work(mxson[x], x);
    printf("YES\n%d\n", k * 2);
    for (int i = 1; i <= k; i++) { printf("%d ", ans[i]); }
    for (int i = k; i >= 1; i--) { printf("%d ", ans[i]); }
    printf("\n");
    return 0;
}