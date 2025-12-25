#include <stdio.h>
#include <algorithm>
#include <vector>
constexpr int N = 300005;
#define Rep(x) for (int i = x; i; --i) 
int a[N];
namespace LCT {
enum PointerType {L, R, FA};
struct SplayNode { // BST，节点的编号就是 val
    int s[3], sz; // FA 记录原树的形态，ch[0/1] 记录辅助树的形态
    bool inv;
    int val, xorsum;
    SplayNode& go(int);
    inline void reverse() { std::swap(s[L], s[R]); inv ^= 1; }
    inline void pushup() {
        sz = go(L).sz + 1 + go(R).sz;
        xorsum = go(L).xorsum ^ val ^ go(R).xorsum;
    }
    inline void pushdown() {
        if (inv) {
            if (s[L]) go(L).reverse();
            if (s[R]) go(R).reverse();
            inv = false;
        }
    }
    inline void set(int x) { val = x; pushup(); }
} tr[N];
inline SplayNode& SplayNode::go(int tp) { return tr[s[tp]]; };
inline bool type(int u) { return tr[u].go(FA).s[R] == u; }
inline bool isroot(int u) { return tr[u].go(FA).s[L] != u && tr[u].go(FA).s[R] != u; }
inline void rotate(int u) {
    auto &i = tr[u];
    int anc = i.go(FA).s[FA];
    bool tp = type(u);
    if (!isroot(i.s[FA])) i.go(FA).go(FA).s[type(i.s[FA])] = u;
    i.go(FA).s[tp] = i.s[!tp]; if (i.s[!tp]) i.go(!tp).s[FA] = i.s[FA];
    i.s[tp] = i.s[FA]; i.go(FA).s[FA] = u; i.s[FA] = anc;
    i.go(tp).pushup(); i.pushup();
}
inline void splay(int u) {
    while (!isroot(u)) {
        if (!isroot(tr[u].s[FA]))
            rotate(type(u) == type(tr[u].s[FA]) ? tr[u].s[FA] : u);
        rotate(u);
    }
}
inline int access(int u) {
    int p = 0;
    for (; u; p = u, u = tr[u].s[FA])
        splay(u), tr[u].s[R] = p, tr[u].pushup();
    return p;
}
void update(int u) { {if (!isroot(u)) update(tr[u].s[FA]);} tr[u].pushdown(); }
inline int begin(int u) { return tr[u].s[L] ? (tr[u].pushdown(), begin(tr[u].s[L])) : u; }
inline int front(int u) { u = begin(u); splay(u); return u; }
inline int getroot(int u) { return front(access(u)); }
inline int lca(int u, int v) { return access(u), access(v); }
inline void changeroot(int u) { tr[access(u)].reverse(); }
inline bool link(int u, int v) {
    if (getroot(u) == getroot(v)) return false;
    changeroot(u); splay(u); tr[u].s[FA] = v;
    return true;
}
inline bool cut(int u, int v) {
    changeroot(u); access(v); splay(v);
    if (tr[v].s[L] != u) return false; // 此时如果有 (u, v) 那么一定是 u->v
    tr[v].s[L] = tr[u].s[FA] = 0; tr[v].pushup();
    return true;
}
inline int query(int u, int v) { return changeroot(u), tr[access(v)].xorsum; }
inline void print(int u, int dep = 0) {
    if (!u) return;
    tr[u].pushdown();
    print(tr[u].s[L], dep + 2);
    printf("(%d):\t", u); Rep(dep) putchar(' '); printf("val=%d, xorsum=%d\n", tr[u].val, tr[u].xorsum);
    print(tr[u].s[R], dep + 2);
}
}
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    auto detailedAnsPrinter = [n] (int v) {
        for (int i = 0; i < n; i++)
            if (v >> i & 1) fprintf(stderr, "%d ", i + 1);
        fputc('\n', stderr);
    };
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    for (int i = 1; i <= n; i++)
        LCT::tr[i].set(a[i]);
    for (int op, x, y, _m = 1; _m <= m; ++_m) {
        scanf("%d%d%d", &op, &x, &y);
        switch (op) {
        case 0:
            printf("%d\n", LCT::query(x, y));
            // {LCT::changeroot(x); int tmp = LCT::access(y);
            // LCT::print(tmp); printf("%d\n", LCT::tr[tmp].xorsum);
            // detailedAnsPrinter(LCT::tr[tmp].xorsum);}
            break;
        case 1:
            LCT::link(x, y) ? fprintf(stderr, "Linked %d to %d on OP #%d.\n", x, y, _m) : fprintf(stderr, "Found same root for node %d and %d on OP #%d.\n", x, y, _m) ;
            break;
        case 2:
            LCT::cut(x, y) ? fprintf(stderr, "Cut %d from %d on OP #%d.\n", x, y, _m) : fprintf(stderr, "Edge (%d, %d) not found on OP #%d.\n", x, y, _m);
            break;
        case 3:
            LCT::splay(x), LCT::tr[x].set(y);
            break;
        }
    }
    return 0;
}