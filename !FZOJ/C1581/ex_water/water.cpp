#include <iostream>
#include <cstdint>
#include <algorithm>
#include <cstring>
constexpr int N = 200005, K = 17;
constexpr uint64_t base = 1ull << 32;
uint64_t h[N], g[N];
int son[N][2], fa[N][K+1];
int monosta[N];
inline int monoins(int x) { // 返回前面第一个比它小的
    while (monosta[0] && h[monosta[monosta[0]]] > h[x]) --monosta[0];
    int res = monosta[monosta[0]];
    monosta[++monosta[0]] = x;
    return res;
}
int lid[N], lfn[N], rid[N], rfn[N]; // 按照 LRU，RLU 的顺序遍历得到的节点序列
int sz[N], sum[N];
int64_t fill[N], fillfa[N], lsum[N], rsum[N]; // 填充当前子树需要最少增水量 / 填充高度到父亲需要的最小增水量
void init(int u) {
    if (!u) return;
    init(son[u][0]), init(son[u][1]);
    sz[u] = 1 + sz[son[u][0]] + sz[son[u][1]];
    fill[u] = fillfa[son[u][0]] + fillfa[son[u][1]];
    fillfa[u] = 1ll * (h[fa[u][0]] - h[u]) * sz[u];
}
void initl(int u) {
    static int ts;
    if (!u) return;
    initl(son[u][0]), initl(son[u][1]);
    lid[++ts] = u, lfn[u] = ts;
}
void initr(int u) {
    static int ts;
    if (!u) return;
    initr(son[u][1]), initr(son[u][0]);
    rid[++ts] = u, rfn[u] = ts;
}
int main() {
    std::cin.tie(nullptr)->sync_with_stdio(false);
    int n, q;
    std::cin >> n;
    for (int i = 1; i <= n; i++) {
        std::cin >> h[i];
        if (son[i][0] = monoins(i))
            fa[son[i][0]][0] = i;
    }
    monosta[0] = 0;
    for (int i = n; i >= 1; i--)
        if (son[i][1] = monoins(i))
            fa[son[i][1]][0] = i;
    int rt = 0;
    for (int i = 1; i <= n; i++)
        if (!fa[i][0]) { rt = i; break; }
    for (int k = 1; k <= K; k++)
        for (int i = 1; i <= n; i++)
            fa[i][k] = fa[fa[i][k-1]][k-1]; 
    initl(rt), initr(rt);
    for (int i = 1; i <= n; i++) lsum[i] = lsum[i-1] + fill[lid[i]];
    for (int i = 1; i <= n; i++) rsum[i] = rsum[i-1] + fill[rid[i]];
    return 0;
}