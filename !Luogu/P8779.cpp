#include <stdio.h>
#include <vector>
#include <numeric>
struct UnionFind {
    std::vector<int> fa;
    std::vector<long long> val; // 到 root(x) 路径上的 sum
    inline UnionFind(int n) : fa(n + 1), val(n + 1) { std::iota(fa.begin(), fa.end(), 0); }
    inline int find(int u) {
        if (u == fa[u]) return u;
        int rt = find(fa[u]);
        val[u] += val[fa[u]];
        fa[u] = rt;
        return rt;
    }
    inline bool unite(int u, int v, long long s) {
        int ru = find(u), rv = find(v);
        if (ru == rv) return false;
        fa[ru] = rv; // u -> ... -> ru <- rv <- ... <- v
        val[ru] = -val[u] + val[v] - s;
        return true;
    }
};
int main() {
    int n, m, q;
    scanf("%d%d%d", &n, &m, &q);
    UnionFind uni(n);
    long long s;
    for (int i = 1, l, r; i <= m; i++) {
        scanf("%d%d%lld", &l, &r, &s);
        uni.unite(l-1, r, s);
    }
    for (int i = 1, l, r; i <= q; i++) {
        scanf("%d%d", &l, &r);
        if (uni.find(l-1) == uni.find(r)) printf("%lld\n", uni.val[r] - uni.val[l-1]);
        else puts("UNKNOWN");
    }
    return 0;
}