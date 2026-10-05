#include <bits/stdc++.h>
char buf[1 << 20], *p1=buf, *p2=buf;
inline char gc() {
    if (p1 == p2) {
        p2 = (p1=buf) + fread(buf, 1, sizeof buf, stdin);
        if (p1 == p2) return EOF;
    }
    return *p1++;
}
template<class T> inline void in(T &x) {
    char ch = gc();
    while (ch < '0' || ch > '9') ch = gc();
    for (x = 0; ch >= '0' && ch <= '9'; ch = gc())
        x = (x << 3) + (x << 1) + (ch ^ '0');
}
#ifdef CLANGD
constexpr int N = 5;
#else
constexpr int N = 1000005;
#endif
std::vector<int> g[N];
int a[N], n, m, k;
namespace BF{
    constexpr int N = 105, K = 6;
    int st[N][K+1], dep[N], dfn[N], top;
    void dfs(int u, int fa) {
        st[dfn[u] = ++top][0] = fa;
        for (int v : g[u]) if (v != fa)
            dep[v] = dep[u] + 1, dfs(v, u);
    }
    inline int min(int u, int v) {
        return dfn[u] < dfn[v] ? u : v;
    }
    inline int lca(int u, int v) {
        if (u == v) return u;
        u = dfn[u], v = dfn[v];
        if (u > v) std::swap(u, v);
        int k = 31 - __builtin_clz(v - u++);
        return min(st[u][k], st[v - (1<<k) + 1][k]);
    }
    int f[N][N];
    inline void work() {
        dfs(1, 0);
        for (int k = 1; k <= K; k++)
            for (int i = 1; i + (1<<k) - 1 <= n; i++)
                st[i][k] = min(st[i][k-1], st[i + (1<<k-1)][k-1]);
        memset(f, 0x3f, sizeof f);
        f[0][0] = 0;
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++)
                fprintf(stderr, "lca(%d,%d)=%d, dep=%d\n", i, j, lca(i, j), dep[lca(i, j)]);
        for (int i = 1; i <= m; i++) {
            int u = a[i];
            for (int l = 1; l <= k; l++)
                for (int j = i; j >= 1; j--) {
                    u = lca(u, a[j]);
                    f[i][l] = std::min(f[i][l], f[j-1][l-1] + dep[u]);
                    fprintf(stderr, "%d, %d, %d: lca=%d, upd %d\n", i, l, j, u, f[j-1][l-1] + dep[u]);
                }
            for (int l = 1; l <= k; l++)
                fprintf(stderr, "%d%c", f[i][l], " \n"[l==k]);
        }
        printf("%d\n", f[m][k]);
    }
}
int main() {
    freopen("celebration.in", "r", stdin);
    freopen("celebration.out", "w", stdout);
    in(n), in(m), in(k);
    for (int i = 1; i <= m; i++) in(a[i]);
    for (int u, v, i = 1; i < n; i++) {
        in(u), in(v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    if (n <= 100) BF::work();
    else {
        std::sort(a+1, a+1+m);
        int64_t ans = 0;
        for (int i = 1; i <= k; i++)
            ans += a[i] - 1;
        printf("%lld\n", ans);
    }
}