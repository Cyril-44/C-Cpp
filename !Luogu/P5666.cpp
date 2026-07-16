#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
constexpr int N = 300000;
std::vector<int> g[N];
int n;
int centroid, size[N];
void getcentroid(int u, int fa) {
    size[u] = 1;
    int mxsz = 0;
    for (int v : g[u]) if (v != fa) {
        getcentroid(v, u);
        size[u] += size[v];
        mxsz = std::max(mxsz, size[v]);
    }
    mxsz = std::max(mxsz, n - size[u]);
    if (mxsz <= n/2) centroid = u;
}
int fa[N], dep[N], hson[N], mxsz[N];
/* 重要性质：
1. 有根树中，[该树的重心] 一定在 {以 (根节点)的(重儿子) 为根的子树}的[重心] 的{祖先}上。
2. 有根树中，[该树的重心] 一定在这颗树的{**任意一条**重链}上。
*/
int subcen[N]; // subcen[u]: 在{以全局重心为根的树}中，{以 u 为根的子树}的**偏下**的[重心]
inline bool isCentroid(int u, int s) {
    return std::max(mxsz[u], s - size[u]) <= s / 2;
}
inline bool isCentroidOf(int u, int rt) { // 判断 u 是不是{以 rt 为根的子树}中的[重心]，需要保证 rt 是 u 的祖先。
    return isCentroid(u, size[rt]);
}
void predfs(int u, int pre) {
    size[u] = 1; mxsz[u] = 0; hson[u] = 0;
    dep[u] = dep[pre] + 1; fa[u] = pre;
    for (int v : g[u]) if (v != pre) {
        predfs(v, u);
        size[u] += size[v];
        if (size[v] > size[hson[u]]) hson[u] = v;
        mxsz[u] = std::max(mxsz[u], size[v]);
    }
    subcen[u] = hson[u] ? subcen[hson[u]] : u;
    while (!isCentroidOf(subcen[u], u)) subcen[u] = fa[subcen[u]];
}
int seHson;
int f1[N]; // f1[i]: 在[根]的[重儿子]的{子树}**外** 剥除一个 {size=i 子树}之后，{剩余的树}的重心
int f2[N]; // f2[i]: 在[根]的[重儿子]的{子树}**内** 删除一个 {size=i 子树}之后，{剩余的树}的重心
/* 我们将删边，看成在树上拎出来一个子树，以及剥去子树的原树。
拎出来的子树的重心，很显然使用 subcen[] 解决。（注意判断其 fa 是不是也是重心）
剥去子树的原树，重心还是在重链上。这里分两种情况考虑。
1. 没有在{根的重儿子的子树}中剥去，此时肯定重链不变，而重心在重链上，只关心除了重链外删了多少个点，即只关心 size。所以预处理 -size 的重心就行了
2. 在{根的重儿子的子树}中剥去。直观理解，如果此时根的重儿子仍然不变，那么重心肯定上移，但是我们已经以重心为根了，所以此时重心不变。
    如果根的重儿子变了，那么重链就彻底变成了 根->重儿子...的这条链。当成方法1做了
*/
inline void initf1() {
    int u = centroid;
    while (hson[u]) u = hson[u];
    for (int i = size[seHson]; i >= 1; i--) {
        while (!isCentroid(u, n - i)) u = fa[u];
        f1[i] = u;
    }
}
inline void initf2() {
    int u = seHson;
    while (hson[u]) u = hson[u];
    for (int i = mxsz[centroid]; i >= 1; i--) {
        if (size[centroid] - i >= size[seHson])
            f2[i] = centroid; // 重心不变
        else {
            while (!isCentroid(u, n - i)) u = fa[u];
            f2[i] = u;
        }
    }
}
int64_t ans;
inline void chk(int cen, int sz, int rt = 0) {
    if (cen != rt) cen = fa[cen];
    if (isCentroid(cen, sz)) ans += cen;
}
void calc(int u, bool flg) { // flg 代表 u 是否位于根的重儿子子树中
    if (u != centroid) {
        ans += subcen[u];
        chk(subcen[u], size[u], u);
        if (flg) {
            ans += f2[size[u]];
            chk(f2[size[u]], n - size[u]);
        } else {
            ans += f1[size[u]];
            chk(f1[size[u]], n - size[u]);
        }
    }
    if (!hson[u]) return;
    calc(hson[u], flg);
    for (int v : g[u]) if (v != fa[u] && v != hson[u])
        calc(v, false);
}
int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        scanf("%d", &n);
        for (int i = 1; i <= n; i++) g[i].clear();
        for (int _=1, u,v; _<n; _++) {
            scanf("%d%d", &u, &v);
            g[u].push_back(v);
            g[v].push_back(u);
        }
        getcentroid(1, 0);
        predfs(centroid, 0);
        seHson = 0;
        for (int v : g[centroid])
            if (v != hson[centroid] && size[v] > size[seHson])
                seHson = v;
        initf1(), initf2();
        ans = 0; calc(centroid, true);
        printf("%lld\n", ans);
    }
    return 0;
}