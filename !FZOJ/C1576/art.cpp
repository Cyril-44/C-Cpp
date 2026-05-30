#include <cstdio>
#include <numeric>
#include <algorithm>
constexpr int N = 100005;
int fa[N], sz[N], cnt[N], val[N];
inline int find(int u) {
    int v = u, tp;
    while (u ^ fa[u]) u = fa[u];
    while (v ^ u) tp = fa[v], fa[v] = u, v = tp;
    return u;
}
int ans;
inline bool unite(int u, int v) {
    u = find(u), v = find(v);
    if (u == v) {
        ans -= val[u];
        val[u] = std::min(++cnt[u], 2);
        ans += val[u];
        return false;
    }
    ans -= val[u] + val[v];
    sz[v] += sz[u], cnt[v] += cnt[u] + 1, fa[u] = v;
    ans += val[v] = std::min(cnt[v], 2);
    return true;
}
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    std::iota(fa+1, fa+1+n, 1);
    std::fill(sz+1, sz+1+n, 1);
    for (int i = 1, u, v; i <= m; i++) {
        scanf("%d%d", &u, &v);
        unite(u, v);
        printf("%d\n", ans);
    }
    return 0;
}