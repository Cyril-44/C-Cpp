#include <stdio.h>
#include <vector>
#include <functional>
#include <algorithm>
constexpr int N = 200005;
int w[N], deg[N], lson[N];
long long fw[N];
std::vector<int> g[N];
bool islson[N];
void dfs(int u) {
    for (int v : g[u]) {
        dfs(v);
        if (fw[v] > fw[lson[u]]) lson[u] = v;
    }
    islson[lson[u]] = true;
    fw[u] = fw[lson[u]] + w[u];
}
int main() {
    int n, k;
    scanf("%d%d", &n, &k);
    for (int i = 1; i <= n; i++)
        scanf("%d", &w[i]);
    for (int u, v, i = 1; i < n; i++) {
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
        ++deg[v];
    }
    int root = 0;
    for (int i = 1; i <= n; i++)
        if (!deg[i]) { root = i; break; }
    /* 贪心：每次选一个叶子，他到根经过的所有点权值和最大，选完把路径上的点 w 赋值为 0
       优化：使用长脸剖分，设置路径长度为其下端点的权值。提取出每一条长链，加权值前 k 大的长链即可
       可并堆：每个节点维护一个大根堆，每次从叶子向上合并，将最大的元素加上当前节点的 w。最后在根取前 k 大的。
    */
    dfs(root);
    std::vector<long long> ws;
    ws.reserve(n);
    for (int i = 1; i <= n; i++)
        if (!islson[i]) ws.push_back(fw[i]);
    k = std::min((int)ws.size(), k);
    std::nth_element(ws.begin(), ws.begin() + k, ws.end(), std::greater<long long>());
    long long ans = 0;
    for (int i = 0; i < k; i++) ans += ws[i];
    printf("%lld\n", ans);
    return 0;
}