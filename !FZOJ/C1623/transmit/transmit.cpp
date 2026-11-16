#include <bits/stdc++.h>
constexpr int N = 100005;
#ifdef CLANGD
#define N 15
#endif
std::vector<int> g[N];
int sz[N];
int lmt, cnt;
void dfs(int u) {
    sz[u] = 1;
    for (int v : g[u]) {
        dfs(v);
        sz[u] += sz[v];
    }
    if (sz[u] > lmt) ++cnt, sz[u] = 0;
}
int main() {
    int n, k;
    scanf("%d%d", &n, &k);
    for (int i = 2; i <= n; i++) {
        int fa; scanf("%d", &fa);
        g[fa].push_back(i);
    }
    dfs(1);
    int l = 0, r = n;
    while (l <= r) {
        int mid = l + r >> 1;
        lmt = mid, cnt = 0, dfs(1);
        if (cnt <= k) r = mid - 1;
        else l = mid + 1;
    }
    printf("%d\n", l);
}