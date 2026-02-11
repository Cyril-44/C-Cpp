#include <stdio.h>
#include <vector>
constexpr int N = 100005, K = 17;
std::vector<int> g[N];
int fa[N];
namespace LCA {
int st[N][K+1];
int dfn[N], ts = 0;
void dfs(int u, int pre) {
    fa[u] = pre;
    st[dfn[u] = ++ts][0] = pre;
    for (int v : g[u]) {
        if (v == pre) continue;
        dfs(v, u);
    }
}
inline int calc(int x, int y) { return dfn[x] < dfn[y] ? x : y; }
inline void init(int n, int rt = 1) {
    dfs(rt, 0);
    for (int k = 1; k <= K; k++)
        for (int i = 1; i + (1 << k) - 1 <= n; i++)
            st[i][k] = calc(st[i][k-1], st[i + (1<<k-1)][k-1]);
}
inline int lca(int x, int y) {
    if (x == y) return x;
    if ((x = dfn[x]) > (y = dfn[y])) std::swap(x, y);
    int k = 31 - __builtin_clz(y - x++);
    return calc(st[x][k], st[y - (1<<k) + 1][k]);
}
} using LCA::lca;
int rt[N];
namespace PSGT {
struct Node { int ls, rs, mx, mxp; };
std::vector<Node> tr(1);
void* initialization = [](){tr.reserve(N * 50); return nullptr;}();
inline int newnode() { tr.emplace_back(); return (int)tr.size() - 1; }
inline void pushup(int u) {
    if (tr[tr[u].ls].mx > tr[tr[u].rs].mx || tr[tr[u].ls].mx == tr[tr[u].rs].mx && tr[tr[u].ls].mxp < tr[tr[u].rs].mxp)
        tr[u].mx = tr[tr[u].ls].mx, tr[u].mxp = tr[tr[u].ls].mxp;
    else
        tr[u].mx = tr[tr[u].rs].mx, tr[u].mxp = tr[tr[u].rs].mxp;
}
void merge(int &u, int v, int l, int r) {
    if (!u || !v) u |= v;
    else if (l == r) tr[u].mx += tr[v].mx;
    else {
        int mid = l + r >> 1;
        merge(tr[u].ls, tr[v].ls, l, mid);
        merge(tr[u].rs, tr[v].rs, mid+1, r);
        pushup(u);
    }
}
void look(int u, int l, int r) {
    if (!u) return;
    if (l == r) printf("%d: (%d, %d)\n", l, tr[u].mx, tr[u].mxp);
    else {
        int mid = l + r >> 1;
        look(tr[u].ls, l, mid);
        look(tr[u].rs, mid+1, r);
    }
}
int P, X;
void upd(int &u, int l, int r) {
    if (!u) u = newnode();
    if (l == r) tr[u].mx += X, tr[u].mxp = l;
    else {
        int mid = l + r >> 1;
        P <= mid ? upd(tr[u].ls, l, mid) : upd(tr[u].rs, mid + 1, r);
        pushup(u);
    }
}
}
int ans[N];
void dfs(int u, int pre) {
    for (int v : g[u]) {
        if (v == pre) continue;
        dfs(v, u);
        PSGT::merge(rt[u], rt[v], 1, 100000);
    }
    // printf("PSGT Rt[%d]:\n", u), PSGT::look(rt[u], 1, 100000);
    ans[u] = PSGT::tr[rt[u]].mxp;
}
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int i = 1, u, v; i < n; i++) {
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    LCA::init(n);
    for (int x, y, z; m--; ) {
        scanf("%d%d%d", &x, &y, &z);
        int anc = lca(x, y);
        // printf("lca(%d,%d)=%d\n", x, y, anc);
        PSGT::P = z, PSGT::X = 1;
        PSGT::upd(rt[x], 1, 100000);
        PSGT::upd(rt[y], 1, 100000);
        PSGT::X = -1;
        PSGT::upd(rt[anc], 1, 100000);
        if (fa[anc]) PSGT::upd(rt[fa[anc]], 1, 100000);
        // printf("PSGT Rt[%d]:\n", x), PSGT::look(rt[x], 1, 100000);
        // printf("PSGT Rt[%d]:\n", y), PSGT::look(rt[y], 1, 100000);
        // printf("PSGT Rt[%d]:\n", anc), PSGT::look(rt[anc], 1, 100000);
    }
    dfs(1, 0);
    for (int i = 1; i <= n; i++)
        printf("%d\n", ans[i]);
    return 0;
}