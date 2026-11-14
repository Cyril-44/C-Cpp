
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

constexpr int N = 500005;
constexpr int B = 22;
constexpr int M = 1 << B;

int n;
vector<int> g[N];

int sz[N], son[N], fa[N];
int dfn[N], bottom[N], totdfn;
int top[N], rnk[N];

int r[N], col[N];
int dep[N], rem[N], mask[N];
int ans[N][3];

// best[mask][k] 存储最大深度  + 1
// 0 表示不存在
int best[M][3];
int kept;

int stk[N], ord[N];

void add(int u) {
    int &v = best[mask[u]][rem[u]];
    v = max(v, dep[u] + 1);
    ++kept;
}

void del(int u) {
    best[mask[u]][rem[u]] = 0;
    --kept;
}

void query(int x, int u) {
    int m = mask[x];

    int v0 = best[m][0];
    int v1 = best[m][1];
    int v2 = best[m][2];

    for (int i = 0; i < B; i++) {
        int *p = best[m ^ (1 << i)];
        v0 = max(v0, p[0]);
        v1 = max(v1, p[1]);
        v2 = max(v2, p[2]);
    }

    int k = rem[x] + rem[u];
    if (k >= 3) k -= 3;

    int d = dep[x] - 2 * dep[u] - 1;

    if (v0) ans[u][k] = max(ans[u][k], d + v0);

    if (++k == 3) k = 0;
    if (v1) ans[u][k] = max(ans[u][k], d + v1);

    if (++k == 3) k = 0;
    if (v2) ans[u][k] = max(ans[u][k], d + v2);
}

// 非递归求子树大小、重儿子、深度与 mask
void dfs0() {
    int cnt = 0, len = 0;
    stk[cnt++] = 1;

    while (cnt) {
        int u = stk[--cnt];
        ord[len++] = u;

        for (int v : g[u]) {
            dep[v] = dep[u] + 1;
            rem[v] = dep[v] % 3;
            mask[v] = mask[u] ^ (1 << col[v]);
            stk[cnt++] = v;
        }
    }

    for (int i = len - 1; i >= 0; i--) {
        int u = ord[i];
        sz[u] = 1;

        for (int v : g[u]) {
            sz[u] += sz[v];
            if (!son[u] || sz[v] > sz[son[u]])
                son[u] = v;
        }
    }
}

// 非递归重链优先 DFS
void dfs1() {
    int cnt = 0;
    stk[cnt++] = 1;

    while (cnt) {
        int u = stk[--cnt];
        int t = u;

        while (u) {
            top[u] = t;
            rnk[dfn[u] = ++totdfn] = u;
            bottom[u] = dfn[u] + sz[u] - 1;

            for (int v : g[u])
                if (v != son[u])
                    stk[cnt++] = v;

            u = son[u];
        }
    }
}

void dsu_on_tree() {
    for (int i = 1; i <= n; i++)
        ans[i][0] = ans[i][1] = ans[i][2] = -1;

    for (int i = totdfn; i >= 1; i--) {
        int u = rnk[i];

        // 继承各儿子子树内部的答案
        for (int v : g[u]) {
            for (int k = 0; k < 3; k++)
                ans[u][k] = max(ans[u][k], ans[v][k]);
        }

        // 重儿子的贡献已经保留
        // 依次合并轻儿子
        for (int v : g[u]) {
            if (v == son[u]) continue;

            // 查询轻儿子与已有节点之间的路径
            if (kept) {
                for (int j = dfn[v]; j <= bottom[v]; j++)
                    query(rnk[j], u);
            }

            // 加入当前轻儿子的所有节点
            for (int j = dfn[v]; j <= bottom[v]; j++)
                add(rnk[j]);
        }

        // 处理以 u 为端点的路径
        if (kept) query(u, u);

        // (u, u) 是合法的长度为 0 的路径
        ans[u][0] = max(ans[u][0], 0);

        add(u);

        // 轻链链顶需要清空整棵子树
        if (u == top[u]) {
            for (int j = dfn[u]; j <= bottom[u]; j++)
                del(rnk[j]);
        }
    }
}

int main() {
    scanf("%d", &n);
    if (!n) return 0;

    for (int i = 1; i <= n; i++)
        scanf("%d", &r[i]);

    for (int i = 2; i <= n; i++) {
        int p;
        char c;
        scanf("%d %c", &p, &c);

        fa[i] = p;
        col[i] = c - 'a';
        g[p].push_back(i);
    }

    dfs0();
    dfs1();
    dsu_on_tree();

    for (int i = 1; i <= n; i++)
        printf("%d%c", ans[i][r[i]], i == n ? '\n' : ' ');

    return 0;
}
