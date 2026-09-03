#include <cstdio>
constexpr int N = 200004;
struct LCT {
    struct Node {
        int fa, ch[2], cnt;
    } tr[N];
    bool isroot(int u) { return tr[tr[u].fa].ch[0] != u && tr[tr[u].fa].ch[1] != u; }
    bool type(int u) { return tr[tr[u].fa].ch[1] == u; }
    void pushup(int u) { tr[u].cnt = tr[tr[u].ch[0]].cnt + 1 + tr[tr[u].ch[1]].cnt; }
    void rotate(int u) {
        int f = tr[u].fa; bool tp = type(u);
        tr[u].fa = tr[f].fa;
        if (!isroot(f)) tr[tr[f].fa].ch[type(f)] = u;
        tr[f].ch[tp] = tr[u].ch[!tp], tr[tr[u].ch[!tp]].fa = f;
        tr[u].ch[!tp] = f, tr[f].fa = u;
        pushup(f);
    }
    void splay(int u) {
        while (!isroot(u)) {
            if (!isroot(tr[u].fa))
                rotate(type(u) != type(tr[u].fa) ? u : tr[u].fa);
            rotate(u);
        }
        pushup(u);
    }
    void access(int u) {
        for (int v = 0; u; u = tr[v = u].fa)
            splay(u), tr[u].ch[1] = v, pushup(u);
    }
} lct;
int main() {
    int n, m; scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        int k; scanf("%d", &k);
        if (i + k <= n) lct.tr[i].fa = i + k;
    }
    scanf("%d", &m);
    for (int i, j, k; m--; ) {
        scanf("%d%d", &i, &j); ++j;
        if (i == 1) {
            lct.access(j), lct.splay(j);
            printf("%d\n", lct.tr[j].cnt);
        } else {
            scanf("%d", &k);
            lct.access(j), lct.splay(j);
            lct.tr[j].ch[0] = lct.tr[lct.tr[j].ch[0]].fa = 0;
            if (j + k <= n) lct.tr[j].fa = j + k;
            lct.pushup(j);
        }
    }
    return 0;
}