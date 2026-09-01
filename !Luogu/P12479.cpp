#include <bits/stdc++.h>
constexpr int N = 200004;
int p[N], w[N];
struct UFS {
    unsigned long long wsum[N];
    int fa[N], sz[N];
    void init(int n) { std::iota(fa+1, fa+1+n, 1), std::fill(sz+1, sz+1+n, 1), std::copy(w+1, w+1+n, wsum+1); }
    int find(int u) { return u == fa[u] ? u : (fa[u] = find(fa[u])); }
    void merge(int u, int v) { fa[u] = v, sz[v] += sz[u], wsum[v] += wsum[u]; }
    double calc(int u) { return 1. * wsum[u] / sz[u]; }
} uf;
/*
妙妙题。
考虑按照权值大小依次考虑节点。发现权值最大的节点一定单独形成一个连通块。
以此为开始，向其父亲拓展。显然父亲加上这个节点一定更优。将该点和父亲合并起来，值取平均作为新点。
不断化为子问题去做，每次取最大的连通块，把根的答案设置为连通块平均值。
注意要主动剔除 find(p[u]) = 0 的僵尸点情况
*/
double ans[N];
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 2; i <= n; i++)
        scanf("%d", &p[i]);
    std::priority_queue<std::pair<double, int>> pq;
    for (int i = 1; i <= n; i++)
        scanf("%d", &w[i]), pq.emplace(w[i], i);
    uf.init(n);
    while (!pq.empty()) {
        auto [wu, u] = pq.top(); pq.pop();
        if (uf.fa[u] != u) continue; // 已经考虑过了
        ans[u] = wu;
        int fa = uf.find(p[u]);
        uf.merge(u, fa);
        if (fa) pq.emplace(uf.calc(fa), fa);
    }
    for (int i = 1; i <= n; i++)
        printf("%.10lf\n", ans[i]);
    return 0;
}