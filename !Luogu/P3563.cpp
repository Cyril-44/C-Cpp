#include <bits/stdc++.h>
constexpr int N = 250005;
#ifdef CLANGD
#define N 25
#endif
std::vector<int> g[N];
int64_t f[N];
int sz[N], h[N], centroid, n;
/*
核心结论：所有点要不然指向重心，要不然指离重心。
证明不会。以后做树的奇怪题可以往重心上猜。
*/
void dfs1(int u, int fa) {
    sz[u] = 1;
    int mxsz = 0;
    for (int v : g[u]) if (v != fa) {
        dfs1(v, u);
        mxsz = std::max(mxsz, sz[v]);
        sz[u] += sz[v];
    }
    mxsz = std::max(mxsz, n - sz[u]);
    if (mxsz <= n/2) centroid = u;
}
void dfs2(int u, int fa) {
    sz[u] = 1;
    for (int v : g[u]) if (v != fa) {
        dfs2(v, u);
        sz[u] += sz[v];
        f[u] += f[v];
    }
    f[u] += sz[u];
}
int main() {
    scanf("%d", &n);
    for (int i = 1, u, v; i < n; i++) {
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    dfs1(1, 0), dfs2(centroid, 0);
    for (int v : g[centroid]) ++h[sz[v]];
    std::bitset<N> dp;
    dp.set(0);
    for (int i = 1; i <= n; i++) {
        for (int k = 0; k <= 20; k++) {
            if (h[i] < (1<<k)) break;
            h[i] -= 1<<k;
            const int l = i * (1<<k);
            dp |= dp << l;
        }
        if (h[i]) dp |= dp << i * h[i];
    }
    int64_t ans = f[centroid] - n;
    for (int i = (n-1)/2; i >= 1; i--)
        if (dp.test(i)) {
            ans += 1ll * i * (n-1-i);
            break;
        }
    printf("%d %lld\n", n-1, ans);
}