#include <stdio.h>
#include <string.h>
#include <algorithm>
#define logprint(...) fprintf(stderr, __VA_ARGS__)
#define umax(x, y) ((x) < (y) ? (x) = (y) : (x))
#define umax3(x, y, z) (umax((x), (y)) < (z) ? (x) = (z) : (x))
constexpr int N = 500005, INF = -4e8;
namespace SDM {
int time_stamp = 0;
struct Node {
    Node *ch[2], *fa;
    int dat, sz;
    int allmx, premx, sufmx, sm, cov;
    int lastupdate = 0;
    bool inv;
    inline Node(Node* f = nullptr, int v = INF, Node *l = nullptr, Node *r = nullptr):
        ch{l, r}, fa(f), dat(v), cov(INF), inv(false) { pushup(); }
    inline ~Node() {
        if (ch[0]) delete ch[0];
        if (ch[1]) delete ch[1];
    }
#define DECL(func, var, fallback) inline int func(bool c) { return ch[c] ? ch[c]->var : fallback; }
    DECL(all, allmx, INF) DECL(pre, premx, 0) DECL(suf, sufmx, 0) DECL(sum, sm, 0) DECL(size, sz, 0)
#undef DECL
    inline bool type() const { return fa->ch[1] == this; }
    inline void reverse() {
        std::swap(ch[0], ch[1]);
        std::swap(premx, sufmx);
        inv ^= 1;
    }
    inline void assign(int x) {
        dat = cov = x;
        sm = sz * x;
        allmx = std::max(sm, x);
        premx = sufmx = std::max(sm, 0);
        inv = false;
    }
    inline void pushdown() {
        if (inv) {
            if (ch[0]) ch[0]->reverse();
            if (ch[1]) ch[1]->reverse();
            inv = false;
        }
        if (cov ^ INF) {
            if (ch[0]) ch[0]->assign(cov);
            if (ch[1]) ch[1]->assign(cov);
            cov = INF;
        }
        lastupdate = ++time_stamp;
    }
    inline void pushup() {
        premx = std::max(pre(0), sum(0) + dat + pre(1));
        sufmx = std::max(suf(1), suf(0) + dat + sum(1));
        allmx = std::max({all(0), suf(0) + dat + pre(1), all(1)});
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
};
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
        Node *u = new Node();
        u->ch[1] = new Node(u); u = u->ch[1];
        for (int i = 1; i <= n; i++) {
            u->ch[1] = new Node(u, a[i]);
            u = u->ch[1];
        }
        u->ch[1] = new Node(u); u = u->ch[1];
        u->ch[1] = new Node(u); u = u->ch[1];
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
        if (rt->ch[1]) splay(rt->ch[1]->begin(), rt);
        else rt->ch[1] = new Node(rt, a[n--]);
        Node *u = rt->ch[1];
        for (int i = n; i >= 1; i--) {
            u->ch[0] = new Node(u, a[i]);
            u = u->ch[0];
        }
        splay(u);
    }
    inline void erase(Node *u) {
        u->fa->ch[u->type()] = nullptr;
        splay(u->fa);
        delete u;
    }
    int totcount;
    inline void dbg(int d = 0) { totcount = -2; dbg(root, d); }
    inline void dbg(Node *u, int dep) {
        if (!u) return;
        // u->pushdown();
        dbg(u->ch[0], dep + 2);
        logprint("#%d\t(%4d) [lupd: %3d] ", ++totcount, u->dat, u->lastupdate);
        for(int i=dep;i--;logprint("."));
        logprint("%s all=%d, pre=%d, suf=%d, sum=%d, sz=%d, cov=%d %s\n", 
u->fa ? u->type() ? "┗" : "┏" : "╋", u->allmx, u->premx, u->sufmx, u->sm, u->sz, u->cov, u->inv ? "inv" : "");
        dbg(u->ch[1], dep + 2);
    }
};
int main() {
#ifndef ONLINE_JUDGE
    freopen("P2042.out", "w", stdout);
    freopen("P2042.txt", "w", stderr);
#endif
    static int a[N];
    int n, m, k;
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    Splay tr(a, n); k = n;
    char operation[16];
    for (int x, y, z; m--; ) {
        x=y=z=114514;
        scanf("%s", operation);
        switch (*operation) {
        case 'I': // Insert
            scanf("%d%d", &x, &y);
            for (int i = 1; i <= y; i++)
                scanf("%d", &a[i]);
            tr.insert(tr.rank(x), a, y);
            k += y;
            break;
        case 'D': // Delete
            scanf("%d%d", &x, &y);
            tr.erase(tr.range(x, y));
            k -= y;
            break;
        case 'R': { // Reverse
            scanf("%d%d", &x, &y);
            auto pos = tr.range(x, y);
            pos->reverse();
            tr.splay(pos);
        }   break;
        case 'G': // Get-sum
            scanf("%d%d", &x, &y);
            if (y == 0) puts("0");
            else printf("%d\n", tr.range(x, y)->sm);
            break;
        case 'M':
            if (operation[2] == 'K') { // Make-same
                scanf("%d%d%d", &x, &y, &z);
                auto pos = tr.range(x, y);
                pos->assign(z);
                tr.splay(pos);
            } else { // Max-sum
                printf("%d\n", tr.root->allmx);
            }
            break;
        }
        // logprint("==================== %s %d %d %d ====================\n", operation, x, y, z); tr.dbg();
    }
    return 0;
}