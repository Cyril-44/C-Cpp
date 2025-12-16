#include <stdio.h>
#include <string.h>
#include <algorithm>
#define umax(x, y) ((x) < (y) ? (x) = (y) : (x))
#define umax3(x, y, z) (umax((x), (y)) < (z) ? (x) = (z) : (x))
constexpr int N = 500005, INF = 0x80000000;
namespace SDM {
static struct Node {
    Node *ch[2], *fa;
    int dat, sz;
    int mx, allmx, premx, sufmx, sm, cov;
    bool inv;
    inline Node(Node* f = nullptr, int v = 0, Node *l = nullptr, Node *r = nullptr):
        ch{l, r}, fa(f), dat(v), sz(1), mx(INF), allmx(0), premx(0), sufmx(0), sm(0), cov(INF), inv(false) {}
#define DECL(func, var, fallback) inline int func(bool c) { return ch[c] ? ch[c]->var : fallback; }
    DECL(max, mx, INF) DECL(all, allmx, 0) DECL(pre, premx, 0) DECL(suf, sufmx, 0) DECL(sum, sm, 0) DECL(size, sz, 0)
#undef DECL
    inline bool type() const { return fa->ch[1] == this; }
    inline void pushdown() {
        if (inv) {
            if (ch[0]) ch[0]->inv ^= 1;
            if (ch[1]) ch[1]->inv ^= 1;
            inv = false;
        }
        if (cov ^ INF) {
            if (ch[0]) ch[0]->cov = cov;
            if (ch[1]) ch[1]->cov = cov;
            dat = mx = cov;
            allmx = premx = sufmx = (cov > 0) ? (cov * sz) : 0;
            cov = INF;
        }
    }
    inline void pushup() {
        premx = pre(0);
        if (pre(0) == all(0)) umax(premx, all(0) + dat + pre(1));
        sufmx = suf(1);
        if (suf(1) == all(1)) umax(sufmx, suf(0) + dat + all(1));
        allmx = suf(0) + dat + pre(1);
        umax3(allmx, pre(0), suf(1));
        mx = std::max(max(0), max(1));
        if (dat < 0) umax(mx, dat);
        sm = sum(0) + dat + sum(1);
        sz = size(0) + 1 + size(1);
    }
    inline Node* begin() { Node *u = this; while (u->pushdown(), u->ch[0]) u = u->ch[0]; return u; }
    inline Node* end() { Node *u = this; while (u->pushdown(), u->ch[1]) u = u->ch[1]; return u; }
    inline void rotate() {
        fa->pushdown(), pushdown();
        auto anc = fa->fa;
        int tp = type(), ftp = anc ? fa->type() : -1;
        fa->ch[tp] = ch[tp^1]; if(ch[tp^1]) ch[tp^1]->fa = fa;
        ch[tp^1] = fa; fa->fa = this;
        if (anc) anc->ch[ftp] = this; fa = anc;
        ch[tp^1]->pushup(), pushup();
    }
} mempool[N];
static size_t counter;
template<typename... Args> inline Node* alloc(Args... args) {
    mempool[counter] = Node(args...);
    return mempool + (counter++);
}
}
struct Splay {
    using Node = SDM::Node;
    Node* root;
    inline void splay(Node *u, Node *goal = nullptr) {
        while (u->fa != goal) {
            if (u->fa->fa != goal)
                u->type() == u->fa->type() ? u->fa->rotate() : u->rotate();
            u->rotate();
        }
        if (!goal) root = u;
    }
    Splay(int *a, int n) {
        Node *u = SDM::alloc();
        u->ch[1] = SDM::alloc(u); u = u->ch[1];
        for (int i = 1; i <= n; i++) {
            u->ch[1] = SDM::alloc(u, a[i]);
            u = u->ch[1];
        }
        u->ch[1] = SDM::alloc(u); u = u->ch[1];
        u->ch[1] = SDM::alloc(u); u = u->ch[1];
        splay(u);
    }
    inline Node* rank(int p) {
        p += 2;
        for (Node *u = root; u->sz; ) {
            u->pushdown();
            if (p <= u->size(0)) u = u->ch[0];
            else if (p <= u->sz - u->size(1)) return u;
            else p -= u->sz - u->size(1), u = u->ch[1];
        }
        throw "Fuck you";
    }
    inline Node* range(int l, int tot) {
        splay(rank(l-1)), splay(rank(l+tot), root);
        return root->ch[1]->ch[0];
    }
    inline void insert(Node *rt, int *a, int n) {
        rt->pushdown();
        if (rt->ch[1]) splay(rt->ch[1]->begin(), rt);
        else rt->ch[1] = SDM::alloc(rt, a[n--]);
        Node *u = rt->ch[1];
        for (int i = n; i >= 1; i--) {
            u->ch[0] = SDM::alloc(u, a[i]);
            u = u->ch[0];
        }
        splay(u);
    }
    inline void erase(Node *u) {
        u->fa->ch[u->type()] = nullptr;
        u->fa->pushup();
    }
    inline void dbg(int d = 0) { dbg(root, d); }
    inline void dbg(Node *u, int dep) {
        if (!u) return;
        dbg(u->ch[0], dep + 2);
        for(int i=dep;i--;putchar(' ')); printf("(%d), mx=%d, all=%d, pre=%d, suf=%d, sz=%d\n", u->dat, u->mx, u->allmx, u->premx, u->sufmx, u->sz);
        dbg(u->ch[1], dep + 2);
    }
};
int main() {
#ifndef ONLINE_JUDGE
    freopen("P2042.in", "r", stdin);
    freopen("P2042.out", "w", stdout);
#endif
    static int a[N];
    int n, m;
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    Splay tr(a, n);
    char operation[16];
    for (int x, y, z; m--; ) {
        scanf("%s", operation);
        switch (*operation) {
        case 'I': // Insert
            scanf("%d%d", &x, &y);
            for (int i = 1; i <= y; i++)
                scanf("%d", &a[i]);
            tr.insert(tr.rank(x), a, y);
            break;
        case 'D': // Delete
            scanf("%d%d", &x, &y);
            tr.erase(tr.range(x, y));
            break;
        case 'R': // Reverse
            scanf("%d%d", &x, &y);
            tr.range(x, y)->inv ^= 1;
            break;
        case 'G': // Get-sum
            scanf("%d%d", &x, &y);
            printf("%d\n", tr.range(x, y)->sm);
            break;
        case 'M':
            if (operation[2] == 'K') { // Make-same
                scanf("%d%d%d", &x, &y, &z);
                tr.range(x, y)->cov = z;
            } else { // Max-sum
                printf("%d\n", std::max(tr.root->allmx, tr.root->mx));
            }
            break;
        }
        printf("========\n"); tr.dbg();
    }
    return 0;
}