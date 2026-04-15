#include <bits/stdc++.h>
constexpr int N = 100005;
std::vector<int> g[N];
int f[N], fa[N], deg[N];
/*
考虑原题的强化，设每个节点访问到时，邻居的权值全部相等，于是可以保证对于每一条边都是相互偷走了一次，最后答案相等。
设这个相等的邻居权值对于每个点来说是 T[u]，设每个点初始权值为 d[u]。
当 u < v 时，d[v] = T[u] + cnt(v's neighbor < u) 即加上损失
当 u > v 时，d[v] = T[u] + cnt(v's neighbor < u) - deg(v) 即加上损失在减去偷盗的。
合并一下有 d[v] = T[u] + cnt(v's < u) - [u > v] deg(v)
转换一下有 T[v] = d[u] - cnt(u's < v) + [u < v] deg(u)
将下式带入上式有 d[v] = d[fa] - cnt(fa's < u) + cnt(v's < u) - [u > v] deg(v) + [u > fa] deg(u)
注意到每次跳两个点，然后考虑现在 d 的值域。
目测每次贡献是 [-deg(v)-deg(u)+2, deg(v)+deg(u)+2]。具体来说也是这样的。证明就是设正的两项为 0 得到最小，设负的两项为 0 得到最大。可能还需要考虑一些边界情况。
直接计算即可。
*/
inline int calc(int u, int v) {
    int res = 0;
    for (int x : g[u]) res += x < v;
    return res;
}
void dfs(int u) {
    for (int v : g[u]) {
        if (v == fa[u]) continue;
        fa[v] = u;
        f[v] = f[fa[u]] - calc(fa[u], u) + calc(v, u) - (u>v) * deg[v] + (u>fa[u]) * deg[fa[u]];
        dfs(v);
    }
}
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1, u, v; i < n; i++) {
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
        g[v].push_back(u);
        ++deg[u], ++deg[v];
    }
    dfs(1);
    const int shift = n - *std::min_element(f+1, f+1+n);
    for (int i = 1; i <= n; i++)
        printf("%d ", f[i] + shift);
    putchar('\n');
    return 0;
}