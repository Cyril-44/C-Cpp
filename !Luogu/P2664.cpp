#include <stdio.h>
#include <vector>
constexpr int N = 100005;
int c[N];
std::vector<int> g[N];
int ts, dfn[N][2], n;
long long sum[N], f[N];
inline void add(int u, int val) {
    f[dfn[u][0]] += val, f[dfn[u][1]+1] -= val;
}
inline int getsize(int u) { return dfn[u][1] - dfn[u][0] + 1; }
int chainNxt[N], chainPreC[N];
std::vector<int> allcs[N], dels[N];

void dfs(int u, int pre) { // 使用树上差分（子树加/子树减），计算当前 u 对所有子树节点的贡献
    dfn[u][0] = ++ts;
    int ppre = chainPreC[c[u]];
    if (ppre) dels[chainNxt[ppre]].push_back(u);
    else allcs[c[u]].push_back(u);
    chainPreC[c[u]] = u;
    for (int v : g[u]) {
        if (v == pre) continue;
        chainNxt[u] = v;
        dfs(v, u);
        int val = getsize(v);
        for (int del : dels[v]) val -= getsize(del);
        val = n - val;
        add(v, val);
        for (int del : dels[v]) add(del, -val);
    }
    dfn[u][1] = ts;
    chainPreC[c[u]] = ppre;
}
int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) scanf("%d", &c[i]);
    for (int u, v, i = 1; i < n; i++) {
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    dfs(1, 0);
    for (int clr = 1; clr <= 100000; clr++) { // 1 单独处理
        int val = 0;
        for (int v : allcs[clr]) val += getsize(v);
        add(1, val);
    }
    for (int i = 1; i <= n; i++) f[i] += f[i-1];
    for (int i = 1; i <= n; i++) printf("%lld\n", n + f[dfn[i][0]]);
    return 0;
}
