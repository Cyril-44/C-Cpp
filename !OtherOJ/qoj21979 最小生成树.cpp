#include <cstdio>
#include <cstring>
#include <algorithm>
constexpr int N = 200005, M = 500005;
int fa[N];
inline int find(int u) { return (u ^ fa[u]) ? (fa[u] = find(fa[u])) : u; }
struct SimpleEdge { int u, v, w; } edgs[M];
inline long long genMST(int n, int m) {
    for (int i = 1; i <= n; i++) fa[i] = i;
    std::sort(edgs+1, edgs+1 + m, [](auto&x, auto&y){return x.w < y.w;});
    long long ret = 0;
    for (int i = 1; i <= m; i++) {
        int fx = find(edgs[i].u), fy = find(edgs[i].v);
        if (fx != fy) fa[fx] = fy, ret += edgs[i].w;
    }
    return ret;
}
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= m; i++)
        scanf("%d%d%d", &edgs[i].u, &edgs[i].v, &edgs[i].w);
    printf("%lld\n", genMST(n, m));
    return 0;
}