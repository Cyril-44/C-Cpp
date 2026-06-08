#include <cstdio>
#include <cstring>
#include <queue>
#include <cassert>
constexpr int N = 100005, LGN = 16;
std::vector<int> g[N];
struct DeletablePQ {
    int top() {
        if (!size()) return -1;
        while (!delel.empty() && allel.top() == delel.top()) allel.pop(), delel.pop();
        return allel.top();
    }
    int pop() { int res = top(); assert(~res); allel.pop(); return res; }
    void del(int x) { delel.push(x); }
    void ins(int x) { allel.push(x); }
    int size() const { return allel.size() - delel.size(); }
    int operator()() {
        if (!size()) return -1;
        int tmp = pop();
        if (!size()) {
            ins(tmp);
            if (!tmp) return 0; // 只有一个点
            return -1;
        }
        int res = tmp + top();
        ins(tmp);
        return res;
    }
private: std::priority_queue<int> allel, delel;
} pq[N], pq2[N], all;
// pq: 存储对于子树 i 为根时，其子树的所有最长链的长度
// pq2: 存储对于子树 i 为根时，树内所有的节点到 fa[i] 的距离
bool vis[N];
namespace TreeDist {
    static int dfn[N], st[N][LGN+1], dep[N], ts;
    void dfs(int u, int pre) {
        st[dfn[u] = ++ts][0] = pre;
        for (int v : g[u]) if (v != pre) dep[v]=dep[u]+1, dfs(v, u);
    }
    inline int min(int x, int y) { return dfn[x] < dfn[y] ? x : y; }
    inline void init(int n) {
        ts = 0; dfs(1, 0);
        for (int k = 1; k <= LGN; k++)
            for (int i = 1; i + (1<<k) - 1 <= n; i++)
                st[i][k] = min(st[i][k-1], st[i + (1<<k-1)][k-1]);
    }
    inline int lca(int u, int v) {
        if (u == v) return u;
        u = dfn[u], v = dfn[v];
        if (u > v) std::swap(u, v);
        int k = 31 - __builtin_clz(v - u++);
        return min(st[u][k], st[v - (1<<k) + 1][k]);
    }
    inline int dist(int u, int v) {
        int _lca = lca(u, v);
        return dep[u] + dep[v] - 2 * dep[_lca];
    }
}
namespace PntDivide {
    int centroid, mnSz, totSz;
    static int sz[N], dis[N][LGN+1];
    bool vis[N];
    void dfs(int u, int fa) {
        sz[u] = 1;
        int mysz = 0;
        for (int v : g[u]) if (v != fa && !vis[v]) {
            dfs(v, u); sz[u] += sz[v];
            mysz = std::max(mysz, sz[v]);
        }
        mysz = std::max(mysz, totSz - sz[u]);
        if (mysz < mnSz) mnSz = mysz, centroid = u;
    }
    void dfs2(int u, int fa) {
        sz[u] = 1;
        for (int v : g[u]) if (!vis[v] && v != fa)
            dfs2(v, u), sz[u] += sz[v];
    }
    int fa[N];
    void initTr(int u, int pre, int nowsz) {
        totSz = mnSz = nowsz, centroid = -1;
        dfs(u, pre); u = centroid; dfs2(u, pre);
        fa[u] = pre, vis[u] = true;
        for (int v : g[u]) if (!vis[v])
            initTr(v, u, sz[v]);
        fprintf(stderr, "%d u=%d\n", pre, u);
    }
    inline void initDist(int n) {
        for (int i = 1; i <= n; i++)
            for (int u = i, j = 0; u; u = fa[u], j++)
                dis[i][j] = TreeDist::dist(i, u);
        for (int i = 1; i <= n; i++)
            for (int u = i, j = 1; fa[u]; u = fa[u], j++)
                pq2[u].ins(dis[i][j]);
        for (int i = 1; i <= n; i++) pq[i].ins(0);
        for (int i = 1; i <= n; i++) if (fa[i])
            pq[fa[i]].ins(pq2[i].top());
        for (int i = 1; i <= n; i++) all.ins(pq[i]());
    }
    bool on[N];
    inline void update(int u) {
        bool nowon;
        all.del(pq[u]());
        on[u] ? (on[u] = false, nowon = false, pq[u].ins(0)) // Turn off
              : (on[u] = true,  nowon = true,  pq[u].del(0)); // Turn on
        all.ins(pq[u]());
        int *ddis = dis[u];
        for (int i = 1; fa[u]; i++, u = fa[u]) {
            all.del(pq[fa[u]]());
            int rem = pq2[u].top();
            nowon ? pq2[u].del(ddis[i])
                  : pq2[u].ins(ddis[i]);
            int upd = pq2[u].top();
            if (~rem) pq[fa[u]].del(rem);
            if (~upd) pq[fa[u]].ins(upd);
            all.ins(pq[fa[u]]());
        }
    }
    inline int inquire() { return all.top(); }
}
// 转化为点分树，然后在点分树上处理修改，每次修改只需要维护 logn 颗子树。
int main() {
    int n, q;
    scanf("%d", &n);
    for (int i = 1, u, v; i < n; i++) {
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    TreeDist::init(n);
    PntDivide::initTr(1, 0, n);
    PntDivide::initDist(n);
    scanf("%d", &q);
    char ch; int x;
    while (q--) {
        scanf(" %c", &ch);
        if (ch == 'C') {
            scanf("%d", &x);
            PntDivide::update(x);// t = int(f[i]/..)
        } else 
            printf("%d\n", PntDivide::inquire());
    }
    return 0;
}