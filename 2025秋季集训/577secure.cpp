#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <queue>
#define Umax(x, y) (x = std::max(x, y))
constexpr int N = 500005, INF = 0x3fffffff;
int n, p[N], k[N];
struct Edge { int to, nxt, w; } edgs[N << 1]; int head[N];
int cnt[N], dep[N], dis[N], hson[N], fa[N]; // dis: 节点 1 到 i 的距离
int dfntop=0, dfn[N], dfnmap[N], disdfn[N];
struct Chain { int head, rear; } chain[N]; // 第 i 个点属于重链: head[i] .. rear[i]
void dfs1(int u, int pre) { // Get cnt, dep, dis, hson, fa
    cnt[u] = 1, fa[u] = pre;
    for (int i = head[u], v; i; i = edgs[i].nxt) {
        if ((v = edgs[i].to) == pre) continue;
        dep[v] = dep[u] + 1, dis[v] = dis[u] + edgs[i].w;
        dfs1(v, u);
        cnt[u] += cnt[v];
        if (cnt[v] > cnt[hson[u]]) hson[u] = v;
    }
}
void dfs2(int u, int top) { // Get dfn(map), chain
    dfn[u] = ++dfntop;
    dfnmap[dfn[u]] = u;
    disdfn[dfn[u]] = dis[u];
    chain[u] = {top, u};
    if (hson[u]) {
        dfs2(hson[u], top);
        chain[u].rear = chain[hson[u]].rear; // 更新链尾
        for (int i = head[u], v; i; i = edgs[i].nxt) {
            if ((v = edgs[i].to) == fa[u] || v == hson[u]) continue;
            dfs2(v, v);
        }
    }
}
namespace SegTree { // 按照 dfn 序编号，不是原编号
    using QueryRes = std::pair<int,int>;
    struct TopSet { // 大根可删堆
        inline void push(int x) { qadd.push(x); }
        inline void erase(int x) { qdel.push(x); }
        inline int top() { // Without no_throw
            while (!qdel.empty() && qadd.top() == qdel.top())
                qadd.pop(), qdel.pop();
            return qadd.empty() ? -INF : qadd.top();
        }
    private:
        std::priority_queue<int> qadd, qdel;
    } base[N << 2];
    int mx[N << 2], mn[N << 2];
    int flglazy[N << 2]; // 覆盖次数（加入 +1，删除 -1）
    bool flg[N << 2]; // 是否被覆盖
    int modify_pos, modify_l, modify_r, modify_val;
    bool modify_type;
    void upd1(int u, int l, int r) { // 更新 (min_dis, max_dis)
        if (l == r) {
            modify_type ? base[u].push(modify_val) : base[u].erase(modify_val);
            mn[u] = dis[u] + base[u].top();
            mx[u] = dis[u] - base[u].top();
        } else {
            int mid = (l + r) >> 1;
            if (modify_pos <= mid) upd1(u<<1, l, mid);
            else upd1(u<<1|1, mid+1, r);
            mx[u] = std::max(mx[u<<1], mx[u<<1|1]);
            mn[u] = std::min(mn[u<<1], mn[u<<1|1]);
        }
    }
    void upd2(int u, int l, int r) { // 更新覆盖状态
        if (modify_l <= l && r <= modify_r)
            flg[u] = flglazy[u] += modify_val; // 看看还有没有可用覆盖了
        else {
            int mid = (l + r) >> 1;
            if (modify_l <= mid) upd2(u<<1, l, mid);
            if (mid < modify_r) upd2(u<<1|1, mid+1, r);
            flg[u] = flg[u<<1] && flg[u<<1|1];
        }
    }
    int query_l, query_r;
    int query1(int u, int l, int r) { // 返回最小值
        if (query_l <= l && r <= query_r) return mn[u];
        int mid = (l + r) >> 1;
        int res = 1 << 30;
        if (query_l <= mid) res = std::min(res, query1(u<<1, l, mid));
        if (mid < query_r) res = std::min(res, query1(u<<1|1, mid+1, r));
        return res;
    }
    int query2(int u, int l, int r) { // 返回最大值
        if (query_l <= l && r <= query_r) return mx[u];
        int mid = (l + r) >> 1;
        int res = 0;
        if (query_l <= mid) res = std::max(res, query2(u<<1, l, mid));
        if (mid < query_r) res = std::max(res, query2(u<<1|1, mid+1, r));
        return res;
    }
    bool query3(int u, int l, int r) {
        if (flg[u]) return true; // 这一整块都合法
        if (query_l <= l && r <= query_r) return false;
        int mid = (l + r) >> 1;
        if (query_l <= mid && !query3(u<<1, l, mid)) return false;
        if (mid < query_r && !query3(u<<1|1, mid+1, r)) return false;
        return true;
    }
    
    /// @brief 更改某一个节点的关键属性
    /// @param u 节点的下标（原顺序，非dfn序）
    /// @param x 设置为`k=x`的关键点
    /// @param f [true] 加入 [false] 删除
    inline void change(int u, int x, bool f) {
        modify_type = f;
        while (u && x >= 0) { // 子树可以直接更新 (dfn 连续)，核心是更新 祖先
            modify_pos = dfn[u], modify_val = x;
            upd1(1, 1, n); // 更新 u 的最大最小
        #define All disdfn + dfn[chain[u].head], disdfn + dfn[chain[u].rear] + 1 // 当前重链上的所有距离(严格递增，按照 dfn 序)
            modify_l = std::lower_bound(All, dis[u] - x) - disdfn; // 二分查找在重链上的影响范围
            modify_r = std::upper_bound(All, dis[u] + x) - disdfn - 1;
            modify_val = f ? 1 : -1;
            if (modify_l <= modify_r) upd2(1, 1, n); // 更新影响范围
        #undef All
            x -= dis[fa[chain[u].head]] - dis[u]; // 减去深度
            u = fa[chain[u].head]; // 跳重链
        }
    }

    /// @brief 查询 u 的最大的影响范围
    /// @param u 节点的下标（原顺序，非dfn序）
    /// @return 
    inline int ask(int u) {
        if (!u) return 0;
        auto res = 0;
        query_l = dfn[chain[u].head], query_r = dfn[u];
        res = std::max(res, dis[u] - query1(1, 1, n)); // 找 u 所在重链中 dfn[u] 上面的
        query_l = dfn[u], query_r = dfn[chain[u].rear];
        res = std::max(res, query2(1, 1, n) - dis[u]); // 找 dfn[u] 下面的
        return res;
    }

    /// @brief 查询 dfn l-r 节点是否`全部`被关键点覆盖
    inline bool askcov(int l, int r) {
        query_l = l, query_r = r;
        return query3(1, 1, n);
    }
} using SegTree::change;
int main() {
#ifndef ONLINE_JUDGE
    freopen("577secure/ex_secure6.in", "r", stdin);
    freopen("577secure/secure.out", "w", stdout);
#else
    freopen("secure.in", "r", stdin);
    freopen("secure.out", "w", stdout);
#endif
    int m, q;
    scanf("%d%d%d", &n, &m, &q);
    for (int i = 1, u, v, w; i < n; i++) {
        scanf("%d%d%d", &u, &v, &w);
        edgs[++head[0]] = Edge {u, head[v], w}; head[v] = head[0];
        edgs[++head[0]] = Edge {v, head[u], w}; head[u] = head[0];
    }
    dep[1] = 1, dfs1(1, 0); dfs2(1, 1);
    
    memset(SegTree::mn, 0x3f, sizeof SegTree::mn);
    for (int i = 1; i <= m; i++) {
        scanf("%d%d", &p[i], &k[i]);
        change(p[i], k[i], true);
    }
    for (int op, x, y; q--; ) {
        scanf("%d%d", &op, &x);
        switch (op) {
        case 1: {
            scanf("%d", &y);
            change(x, y, true);
            ++m, p[m] = x, k[m] = y;
            break;
        }
        case 2: {
            change(p[x], k[x], false);
            break;
        }
        case 3: {
            scanf("%d", &y);
            static struct { int node/*每次跳重链经过的点*/, k; } stack_x[N], stack_y[N];
            int top_x = 0, top_y = 0;
            while (chain[x].head != chain[y].head) { // 跳重链求lca，记录每次经过的点
            #define Update(u) do { \
                ++top_##u; \
                stack_##u[top_##u].node = u; \
                stack_##u[top_##u].k = SegTree::ask(u); /*存储上面重链的最大影响范围*/ \
                u = fa[chain[u].head]; \
            } while (0)
                if (dep[x] > dep[y]) Update(x);
                else Update(y);
            #undef Update
            }
            if (dep[x] > dep[y]) std::swap(x, y); // 假设 x 为 lca (对后序操作无任何影响)
            int lca = x, max_impact = 0;
            while (x = fa[chain[x].head])
                Umax(max_impact, SegTree::ask(x) - (dis[fa[lca]] - dis[x])); // 祖先安全点的最大贡献
            x = lca;
            if (max_impact < dis[y] - dis[fa[x]] && !SegTree::askcov(std::upper_bound(disdfn + dfn[x], disdfn + dfn[y] + 1, dis[fa[x]] + max_impact) - disdfn, dfn[y]))
                { puts("No"); break; }
            bool flg = true;
            #define Do(x) do { \
                Umax(stack_##x[top_##x].k, max_impact - (dis[fa[chain[stack_##x[top_##x].node].head]] - dis[fa[lca]])); \
                while (top_##x > 0) { /*自上而下处理*/ \
                    int v = stack_##x[top_##x].node; /*当前处理节点*/ \
                    int u = chain[v].head; /*所在链的顶端*/ \
                    int len = dis[v] - dis[fa[u]]; /*节点到上一条重链的距离*/ \
                    max_impact = stack_##x[top_##x].k; \
                    if (max_impact < len && !SegTree::askcov(std::upper_bound(disdfn + dfn[u], disdfn + dfn[v] + 1, dis[fa[u]] + max_impact) - disdfn, dfn[v])) \
                        { flg = false; break; } /*不仅祖先没有贡献，该重链和所有轻链也没有贡献*/ \
                    --top_##x; \
                    Umax(stack_##x[top_##x].k, max_impact - len); /*向下更新*/ \
                } \
            } while (0)
            Do(x); if (flg) Do(y);
            puts(flg ? "Yes" : "No");
        }
        }
    }
    return 0;
}

