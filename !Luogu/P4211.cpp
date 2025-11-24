#include <stdio.h>
#include <string.h>
#include <vector>
#include <algorithm>
constexpr int N = 50005;
std::vector<int> g[N];
struct Node { int dfn, top, fa, hson, size, dep; } tr[N];
struct Query {
    int w, u, v, id;
    inline bool operator< (const Query &rhs) const {
        return u < rhs.u;
    }
} que[N << 1];
namespace HLD {
    void dfs1(int u) {
        tr[u].size = 1;
        int mxs=0, mxp=0;
        for (int v : g[u]) {
            tr[v].fa = u;
            tr[v].dep = tr[u].dep + 1;
            dfs1(v);
            tr[u].size += tr[v].size;
            if (tr[v].size > mxs)
                mxp = v, mxs = tr[v].size;
        }
        tr[u].hson = mxp;
    }
    int timestamp = 0;
    void dfs2(int u) {
        tr[u].dfn = ++timestamp;
        if (tr[u].hson) {
            tr[tr[u].hson].top = tr[u].top;
            dfs2(tr[u].hson);
            for (int v : g[u]) {
                if (v == tr[u].hson) continue;
                tr[v].top = v;
                dfs2(v);
            }
        }
    }
    inline void hld() { dfs1(1); tr[1].top = 1; dfs2(1); }
}
int n;
namespace SegTr {
    struct { long long sum; int lazy; } tr[N << 2];
    int L, R;
    inline void pushdown(int u, int ls, int rs) {
        if (tr[u].lazy) {
            tr[u<<1].lazy += tr[u].lazy;
            tr[u<<1|1].lazy += tr[u].lazy;
            tr[u<<1].sum += 1ll * tr[u].lazy * ls;
            tr[u<<1|1].sum += 1ll * tr[u].lazy * rs;
            tr[u].lazy = 0;
        }
    }
    void upd(int u, int l, int r) {
        if (L <= l && r <= R) {
            tr[u].sum += r - l + 1;
            tr[u].lazy++;
            return;
        }
        int mid = l + r >> 1;
        pushdown(u, mid - l + 1, r - mid);
        if (L <= mid) upd(u << 1, l, mid);
        if (mid < R) upd(u << 1 | 1, mid + 1, r);
        tr[u].sum = tr[u<<1].sum + tr[u<<1|1].sum;
    }
    long long query(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u].sum;
        int mid = l + r >> 1;
        long long res = 0;
        pushdown(u, mid-l+1, r-mid);
        if (L <= mid) res += query(u << 1, l, mid);
        if (mid < R) res += query(u << 1 | 1, mid + 1, r);
        return res;
    }
    inline void change(int l, int r) {
        L = l, R = r;
        upd(1, 1, n);
    }
    inline long long ask(int l, int r) {
        L = l, R = r;
        return query(1, 1, n);
    }
}
long long ans[N];
inline void add(int u) {
    while (u) {
        SegTr::change(tr[tr[u].top].dfn, tr[u].dfn);
        u = tr[tr[u].top].fa;
    }
}
inline long long get(int u) {
    long long res = 0;
    while (u) {
#ifdef DEBUG
        fprintf(stderr, "+%lld(%d) ", SegTr::ask(tr[tr[u].top].dfn, tr[u].dfn), u);
#endif
        res += SegTr::ask(tr[tr[u].top].dfn, tr[u].dfn);
        u = tr[tr[u].top].fa;
    }
#ifdef DEBUG
    fprintf(stderr, "\n");
#endif
    return res;
}
int main() {
    int m;
    scanf("%d%d", &n, &m);
    for (int i = 2, u; i <= n; i++) {
        scanf("%d", &u);
        g[u+1].push_back(i);
    }
    HLD::hld();
    for (int l, r, z, i=1; i<=m; i++) {
        scanf("%d%d%d", &l, &r, &z);
        ++l, ++r, ++z;
        que[i*2-1] = {-1, l-1, z, i};
        que[i*2] = {1, r, z, i};
    }
    std::sort(que+1, que+1 + m * 2);
    int ptr = 1;
    while (que[ptr].u == 0) ++ptr;
    for (int i = 1; i <= n; i++) {
        add(i);
        while (que[ptr].u == i) {
            ans[que[ptr].id] += que[ptr].w * get(que[ptr].v);
            ++ptr;
        }
    }
    for (int i = 1; i <= m; i++)
        printf("%lld\n", ans[i] % 201314);
    return 0;
}
