#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
constexpr int N = 100005;
std::vector<int> g[N];
int hson[N], size[N], fa[N];
// bool on[N];
void dfsInit(int u, int pre) {
    size[u] = 1;
    // on[u] = true;
    for (int v : g[u]) if (v != pre) {
        fa[v] = u;
        dfsInit(v, u);
        size[u] += size[v];
        if (size[v] > size[hson[u]]) hson[u] = v;
    }
}
struct QTree {
    struct Node {
        int ls, rs, fa;
        int thisl, suml; // light-edge connected nodes
        bool on, tag, allon, hason;
        inline void flip() {
            on ^= 1, tag ^= 1;
            if (allon) allon = hason = false;
            else if (!hason) allon = hason = true;
        }
    } tr[N];
    QTree() { tr[0].allon = true; }
    inline void pushup(int u) {
        tr[u].suml = tr[tr[u].ls].suml + tr[u].thisl + tr[tr[u].rs].suml;
        tr[u].allon = tr[tr[u].ls].allon & tr[u].on & tr[tr[u].rs].allon;
        tr[u].hason = tr[tr[u].ls].hason | tr[u].on | tr[tr[u].rs].hason;
    }
    inline void pushdown(int u) {
        if (tr[u].tag) {
            if (tr[u].ls) tr[tr[u].ls].flip();
            if (tr[u].rs) tr[tr[u].rs].flip();
            tr[u].tag = false;
        }
    }
    int chain[N], chainPreSum[N];
    int buildChain(int l, int r) {
        int mids = chainPreSum[l-1] + chainPreSum[r] >> 1;
        int mid = std::lower_bound(chainPreSum+l, chainPreSum+r+1, mids) - chainPreSum;
        int u = chain[mid];
        if (l ^ mid) tr[u].ls = buildChain(l, mid - 1), tr[tr[u].ls].fa = u;
        if (mid ^ r) tr[u].rs = buildChain(mid + 1, r), tr[tr[u].rs].fa = u;
        tr[u].thisl = size[u] - size[hson[u]], tr[u].on = true;
        pushup(u);
        return u;
    }
    int build(int u) {
        int v = u;
        do for (int w : g[v])
            if (w != fa[v] && w != hson[v])
                tr[build(w)].fa = v;
        while (v = hson[v]);
        v = u; int c = 0;
        do chain[++c] = v, chainPreSum[c] = chainPreSum[c-1] + size[v] - size[hson[v]];
        while (v = hson[v]);
        return buildChain(1, c);
    }
    inline void sync(int u) {
        if (tr[u].fa) sync(tr[u].fa);
        pushdown(u);
    }
    inline int getFront(int u) {
        int res = 0;
        while (u) {
            pushdown(u);
            if (!tr[u].hason) break;
            if (tr[tr[u].ls].allon) {
                res += tr[tr[u].ls].suml;
                if (!tr[u].on) break;
                res += tr[u].thisl, u = tr[u].rs;
            }
            else u = tr[u].ls;
        }
        return res;
    }
    inline int getBack(int u) {
        int res = 0;
        while (u) {
            pushdown(u);
            if (!tr[u].hason) break;
            if (tr[tr[u].rs].allon) {
                res += tr[tr[u].rs].suml;
                if (!tr[u].on) break;
                res += tr[u].thisl, u = tr[u].ls;
            }
            else u = tr[u].rs;
        }
        return res;
    }
    inline int inquire(int u) {
        sync(u);
        bool lastjmp = true;
        for (; tr[u].on; u = tr[u].fa) { //找最上面的一个黑边
            if (lastjmp && !tr[tr[u].ls].allon) break;
            lastjmp = tr[tr[u].fa].rs == u || tr[tr[u].fa].ls != u && tr[tr[u].fa].rs != u; // 从右边跳过来 或者 跳重链，那是往上走
        }
        printf("LastNode = %d, L(%d) + M(%d) + R(%d)\n", u, getBack(tr[u].ls), tr[u].thisl, getFront(tr[u].rs));
        return getBack(tr[u].ls) + getFront(tr[u].rs) + tr[u].thisl;
    }
    inline void update(int u) {
        sync(u);
        bool lastjmp = true;
        for (int v = u; v; v = tr[v].fa)
            if (tr[v].fa && tr[tr[v].fa].ls != v && tr[tr[v].fa].rs != v) {
                tr[tr[v].fa].thisl -= getFront(v);
                printf("(sum[%d] -= %d)", tr[v].fa, getFront(v));
            }
        for (; u; u = tr[u].fa) {
            if (lastjmp) {
                if (tr[u].ls) tr[tr[u].ls].flip();
                tr[u].on ^= 1;
            }
            pushup(u);
            lastjmp = tr[tr[u].fa].rs == u;
            if (tr[u].fa && tr[tr[u].fa].ls != u && tr[tr[u].fa].rs != u) {
                lastjmp = true;
                tr[tr[u].fa].thisl += getFront(u);
            }
        }
    }
} bst;
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int u, v, i = 1; i < n; i++) {
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    dfsInit(1, 0);
    int rt = bst.build(1);
    for (int i = 1; i <= n; i++)
        printf("Node %d connected with %d nodes.\n", i, bst.inquire(i));
    for (int op, u, v; m--; ) {
        scanf("%d%d", &op, &u);
        if (op == 2) {
            printf("%d\n", bst.inquire(u));
        } else {
            scanf("%d", &v);
            bst.update(u);
            for (int i = 1; i <= n; i++)
                printf("(%d==>%d)", i, bst.inquire(i));
            bst.update(v);
            for (int i = 1; i <= n; i++)
                printf("(%d==>%d)", i, bst.inquire(i));
        }
    }
    return 0;
}