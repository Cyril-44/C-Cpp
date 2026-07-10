#include <vector>
#include <algorithm>
#include <fstream>
#include <cassert>
#include <array>
#include <iostream>
using Vec = std::array<int64_t, 2>;
#define For(i, s, t) for (int i = (s); i <= (t); i++)
#define roF(i, s, t) for (int i = (s); i >= (t); i--)
constexpr int N = (int)5e5+5, M = (int)1e6+5, LG = 19, Q = (int)1e5+5;
constexpr int64_t INF = 1e18;
std::ostream& operator<<(std::ostream& os, const Vec& v) { return os << '{' << v[0] << ',' << v[1] << '}'; }
int64_t ans[M];
struct Edge {
    int u, v, w;
    friend bool operator<(const Edge& x, const Edge& y) {
        return x.u < y.u || x.u == y.u && x.v > y.v;
    }
} edg[M];
int baseid[N], dep[M], pre[M], suf[M];
std::vector<std::array<int, LG+1>> fa;
struct Mat { // 维护 min+ 矩阵 {{左->左，左->右}，{右->左，右->右}}
    int64_t a[2][2] = {{0, INF}, {INF, 0}};
    int64_t* operator[](bool x) { return a[x]; }
    const int64_t* operator[](bool x) const { return a[x]; }
    friend Mat operator*(const Mat& x, const Mat& y) {
        Mat z{{{INF,INF},{INF,INF}}};
        For(k, 0, 1) For(i, 0, 1) For(j, 0, 1)
            z[i][j] = std::min(z[i][j], x[i][k] + y[k][j]);
        return z;
    }
    Mat operator*=(const Mat &o) { return *this = *this * o; }
};
std::vector<std::array<Mat, LG+1>> f;
Vec operator*(const Mat& x, const Vec& y) {
    Vec res{INF, INF};
    For(k, 0, 1) For(i, 0, 1) // 注意这里是 y^T * x
        res[i] = std::min(res[i], x[k][i] + y[k]);
    return res;
}
int64_t presum[M], sufsum[M];
struct Node {
    std::vector<int> sons;
    int operator[](int p) const { return sons[p]; }
    int64_t dis; // 从左端点到右端点经过的最小长度
    int sz() const { return sons.size(); }
} tr[M];
/* Key Observation:
如果从 u->v，经过了包含于[u,v]的边，那么，这一段路径一定不会经过包含[u,v]的边。
反映到包含树上，就是如果 u->v 从儿子走的，一定不从父亲走。
*/
void dfs(int u) { // 从下往上搞出来转移矩阵
    int64_t sondis = 0;
    for (int v : tr[u].sons) {
        dep[v] = dep[u] + 1; dfs(v);
        sondis += tr[v].dis;
        presum[v] = sufsum[v] = tr[v].dis;
    }
    if (sondis) tr[u].dis = std::min(tr[u].dis, sondis);
    For(i, 1, tr[u].sz()-1) pre[tr[u][i]] = tr[u][i-1], presum[tr[u][i]] += presum[tr[u][i-1]];
    roF(i, tr[u].sz()-2, 0) suf[tr[u][i]] = tr[u][i+1], sufsum[tr[u][i]] += sufsum[tr[u][i+1]];
}
void dfs2(int u, int l = 0) { // 从上往下，利用转移矩阵带入这条边，得到这条边走父亲最小的路径长度
    // For(_,1,l) putchar(' '); printf("[%d,%d]: %ld\n", edg[u].u, edg[u].v, tr[u].dis);
    for (int k = 1; dep[u] >> k; k++)
        fa[u][k] = fa[fa[u][k-1]][k-1],
        f[u][k] = f[u][k-1] * f[fa[u][k-1]][k-1];
    For(i, 0, tr[u].sz()-1) { // 注意可以走两边。
        int v = tr[u].sons[i];
        f[v][0] = {{{std::min(presum[pre[v]], tr[u].dis + sufsum[v]), std::min(sufsum[v], tr[u].dis + presum[pre[v]])}, 
                    {std::min(presum[v], tr[u].dis + sufsum[suf[v]]), std::min(sufsum[suf[v]], tr[u].dis + presum[v])}}};
        tr[v].dis = std::min(tr[v].dis, tr[u].dis + (i ? presum[tr[u][i-1]] : 0) + (i!=tr[u].sz()-1 ? sufsum[tr[u][i+1]] : 0)); // 绕大圈
        // printf("%d <- %ld + %ld + %ld\n", v, tr[u].dis, (i ? pre[tr[u][i-1]] : 0), (i!=tr[u].sz()-1 ? suf[tr[u][i+1]] : 0));
        dfs2(v, l + 2);
    }
}
int main() {
    std::ifstream fin("chain.in");
    std::ofstream fout("chain.out");
    int n, m, q; fin >> n >> m >> q;
    For (i, 1, m) {
        fin >> edg[i].u >> edg[i].v >> edg[i].w;
        if (edg[i].u > edg[i].v) std::swap(edg[i].u, edg[i].v);
    }
    fa.resize(m+1); f.resize(m+1);
    edg[0].u = 1, edg[0].v = n;
    std::sort(edg+1, edg+1+m);
    {
        static int sta[N];
        int top = 0;
        sta[++top] = 0;
        For(i, 1, m) {
            tr[i].dis = edg[i].w;
            if (edg[i].v - edg[i].u == 1) baseid[edg[i].u] = i;
            while (edg[i].u >= edg[sta[top]].v) --top;
            fa[i][0] = sta[top];
            tr[sta[top]].sons.push_back(i);
            sta[++top] = i;
        }
        tr[0].dis = INF;
        dfs(0);
        dfs2(0);
    }
    // std::cout << std::flush;
    For(_q, 1, q) {
        int x, y;
        fin >> x >> y; // 将点转化为边，从 x-->y 的左端点走到右端点。
        if (x > y) std::swap(x, y);
        --y;
        x = baseid[x], y = baseid[y];
        if (x == y) fout << tr[x].dis << '\n';
        else { // 想要跳出一个区间，一定要经过端点。根据这个原则，我们一层一层往上跳，直到两个区间到 LCA.
            Mat fx, fy;
            Vec basex{0, tr[x].dis}, basey{tr[y].dis, 0};
            // printf("\nQuery %d [%d,%d] [%d,%d]:\n", _q, edg[x].u, edg[x].v, edg[y].u, edg[y].v);
            if (dep[y] > dep[x]) {
                roF(k, LG, 0)
                    if (dep[y] - dep[x] >> k & 1) {
                        fy *= f[y][k], y = fa[y][k];
                        // printf("Jump fy:{%4ld,%4ld} (Fly +%d)\n        {%4ld,%4ld}\n", fy[0][0], fy[0][1], 1<<k, fy[1][0], fy[1][1]);
                    }
            } else if (dep[x] > dep[y]) {
                roF(k, LG, 0)
                    if (dep[x] - dep[y] >> k & 1) {
                        fx *= f[x][k], x = fa[x][k];
                        // printf("Jump fx:{%4ld,%4ld} (Fly +%d)\n        {%4ld,%4ld}\n", fx[0][0], fx[0][1], 1<<k, fx[1][0], fx[1][1]);
                    }
            }
            roF(k, LG, 0)
                if (fa[x][k] != fa[y][k]) {
                    fx *= f[x][k], fy *= f[y][k], x = fa[x][k], y = fa[y][k];
                    // printf("Jump fx:{%4ld,%4ld} fy:{%4ld,%4ld}\n        {%4ld,%4ld}    {%4ld,%4ld}\n", fx[0][0], fx[0][1], fy[0][0], fy[0][1], fx[0][0], fx[0][1], fy[0][0], fy[0][1]);
                }
            /* 考虑两种情况，一种是 x ==> x'(现在的x)的左端 ==> lca的左端 ==> lca的右端 ==> y'(现在的y)的右端 ==> y
               一种是 x ==> x'的右端 ==> y'的左端 ==> y */
            Mat gx = fx * f[x][0], gy = fy * f[y][0];
            Vec vfx = fx * basex, vfy = fy * basey, vgx = gx * basex, vgy = gy * basey;
            // printf("Jump to [%d,%d] [%d,%d]\n   trans gx:{%4ld,%4ld} gy:{%4ld,%4ld} fx:{%4ld,%4ld} fy:{%4ld,%4ld}\n            {%4ld,%4ld}    {%4ld,%4ld}    {%4ld,%4ld}    {%4ld,%4ld}\n", edg[x].u, edg[x].v, edg[y].u, edg[y].v, gx[0][0], gx[0][1], gy[0][0], gy[0][1], fx[0][0], fx[0][1], fy[0][0], fy[0][1], gx[1][0], gx[1][1], gy[1][0], gy[1][1], fx[1][0], fx[1][1], fy[1][0], fy[1][1]);
            // std::cout << "\ttrans vgx:" << vgx << ", vgy:" << vgy << ", vfx:" << vfx << ", vfy:" << vfy << std::endl;
            int lca = fa[x][0];
            assert(pre[y]);
            int64_t dis1 = vgx[0] + vgy[0],
                    dis2 = vgx[1] + vgy[1],
                    dis3 = vgx[0] + tr[lca].dis + vgy[1],
                    dis4 = vfx[1] + presum[pre[y]] - presum[x] + vfy[0];
            // printf("%ld %ld %ld %ld\n", dis1, dis2, dis3, dis4);
            fout << std::min({dis1, dis2, dis3, dis4}) << '\n';
        }
    }
    return 0;
}
