#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#include <bitset>
#include <functional>
#include <limits>
constexpr int N = 1000005;
int a[N], bfn[N], dfn[N][2], dfn2v[N], last[N];
std::vector<int> g[N];
std::vector<int> layer[N];
struct Query { int x, op; bool upd; } q[N];
std::bitset<N> used[2];
int n, ts;

template<typename Comp>
struct SegTr {
    int tr[N << 2];
    int ql, qr, p, x;
    inline int comp(int x, int y) {
        return Comp()(x, y) ? x : y;
    }
    inline void pushup(int u) {
        tr[u] = comp(tr[u << 1], tr[u << 1 | 1]);
    }
    void build(int u=1, int l=1, int r=n) {
        if (l == r) tr[u] = a[dfn2v[l]];
        else {
            int mid = l + r >> 1;
            build(u << 1, l, mid);
            build(u << 1 | 1, mid + 1, r);
            pushup(u);
        }
    }
    void upd(int u, int l, int r) {
        if (l == r) tr[u] = x;
        else {
            int mid = l + r >> 1;
            if (p <= mid) upd(u << 1, l, mid);
            else upd(u << 1 | 1, mid + 1, r);
            pushup(u);
        }
    }
    int inq(int u, int l, int r) {
        if (ql <= l && r <= qr) return tr[u];
        int mid = l + r >> 1;
        if (qr <= mid) return inq(u << 1, l, mid);
        if (ql > mid) return inq(u << 1 | 1, mid + 1, r);
        return comp(inq(u << 1, l, mid), inq(u << 1 | 1, mid + 1, r));
    }
    inline void update(int pos, int val) { p = pos, x = val; upd(1, 1, n); }
    inline int inquire(int l, int r) { ql = l, qr = r; return inq(1, 1, n); }
};
SegTr<std::less<int>> fmn;
SegTr<std::greater<int>> fmx;

inline void bfs(int s) {
    static int que[N];
    int hd=0, tl=0;
    que[tl++] = s;
    bfn[s] = 1;
    while (hd ^ tl) {
        int u = que[hd++];
        for (int v : g[u]) {
            que[tl++] = v;
            bfn[v] = bfn[u] + 1;
        }
    }
}
void dfs(int u) {
    dfn[u][0] = ++ts;
    dfn2v[ts] = u;
    for (int v : g[u]) dfs(v);
    dfn[u][1] = ts;
}
int main() {
    // freopen("ex_spider3.in", "r", stdin);
    int m;
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    for (int u, v = 2; v <= n; v++) {
        scanf("%d", &u);
        g[u].push_back(v);
    }
    for (int i = 1; i <= m; i++) {
        scanf("%d%d", &q[i].x, &q[i].op);
        q[i].upd = true;
        if (last[q[i].x]) q[last[q[i].x]].upd = false;
    }
    bfs(1);
    for (int i = 1; i <= n; i++)
        layer[bfn[i]].push_back(i);
    dfs(1);
    fmn.build(), fmx.build();
    if (m <= 2000) {
        for (int i = 1; i <= m; i++) {
            for (int u : layer[q[i].x]) {
                a[u] = q[i].op ? fmx.inquire(dfn[u][0], dfn[u][1]) : fmn.inquire(dfn[u][0], dfn[u][1]);
                fmn.update(dfn[u][0], a[u]), fmx.update(dfn[u][0], a[u]);
            }
        }
        printf("%d\n", a[1]);
        return 0;
    }
    for (int i = 1; i <= m; i++) {
        if (!used[q[i].op][q[i].x]) {
            used[q[i].op][q[i].x] = true;
            if (q[i].x == bfn[n]) continue;
            for (int u : layer[q[i].x])
                if (q[i].op) fmn.update(dfn[u][0], std::numeric_limits<int>::max());
                else fmx.update(dfn[u][0], std::numeric_limits<int>::min());
        }
        if (q[i].upd) {
            for (int u : layer[q[i].x]) {
                a[u] = q[i].op ? fmx.inquire(dfn[u][0], dfn[u][1]) : fmn.inquire(dfn[u][0], dfn[u][1]);
                fmn.update(dfn[u][0], a[u]);
                fmx.update(dfn[u][0], a[u]);
            }
        }
    }
    printf("%d\n", a[1]);
    return 0;
}
