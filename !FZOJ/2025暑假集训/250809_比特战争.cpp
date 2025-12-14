#include <cstdio>
#include <cstring>
#include <algorithm>
constexpr int N = 100005, M = 200005;
int a[N], b[N];
namespace DSU {
    struct Dsu {
        int fa, mxa, mnb;
        long long ans;
    } d[N];
    inline void init(const int &n) {
        for (int i = 1; i <= n; i++)
            d[i] = {i, a[i], b[i], a[i] * b[i]};
    }
    inline int getroot(int u) {
        return (u ^ d[u].fa) ? (d[u].fa = getroot(d[u].fa)) : u;
    }
    inline bool merge(int u, int v, const int &w) {
        u = getroot(u), v = getroot(v);
        if (u ^ v) {
            d[v].fa = u;
            d[u].mnb = std::min(d[v].mnb, d[u].mnb);
            d[u].mxa = std::max(d[v].mxa, d[u].mxa);
            d[u].ans = std::min(d[u].ans + d[v].ans, 1ll * std::max(w, d[u].mxa) * d[u].mnb);
            return true;
        }
        return false;
    }
}
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++)
        scanf("%d%d", &a[i], &b[i]);
    static struct Edge { int u, v, w; } e[M];
    for (int i = 1; i <= m; i++)
        scanf("%d%d%d", &e[i].u, &e[i].v, &e[i].w);
    std::sort(e+1, e+1 + m, [](Edge&x, Edge&y){return x.w < y.w;});
    DSU::init(n);
    for (int i = 1; i <= m; i++)
        DSU::merge(e[i].u, e[i].v, e[i].w);
    long long sum = 0;
    for (int i = 1; i <= n; i++)
        if (DSU::d[i].fa == i)
            sum += DSU::d[i].ans;
    printf("%lld\n", sum);
    return 0;
}