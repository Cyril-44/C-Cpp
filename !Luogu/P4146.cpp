#include <stdio.h>
#include <assert.h>
#include <algorithm>
constexpr int N = 50005;
#ifdef DEBUG
#define logprint(...) fprintf(stderr, __VA_ARGS__)
#else
#define logprint(...) ((void)0)
#endif
#define umax(x, y) ((x) < (y) ? (x) = (y) : (x))
namespace SDM {
static struct Node {
    Node *ch[2], *fa;
    int size, max, dat;
    int add; bool inv;
    inline bool type() const { return fa->ch[1] == this; }
    inline int sz(bool c) const { return ch[c] ? ch[c]->size : 0; }
    inline int mx(bool c) const { return ch[c] ? ch[c]->max : -0x7fffffff; }
    inline void pushup() {
        size = sz(0) + 1 + sz(1);
        max = dat, umax(max, mx(0)), umax(max, mx(1));
    }
    inline void plus(int x) { dat += x, add += x, max += x; }
    inline void invert() { inv ^= 1, std::swap(ch[0], ch[1]); }
    inline void pushdown() {
        if (add) {
            if (ch[0]) ch[0]->plus(add);
            if (ch[1]) ch[1]->plus(add);
            add = 0;
        }
        if (inv) {
            if (ch[0]) ch[0]->invert();
            if (ch[1]) ch[1]->invert();
            inv = false;
        }
    }
    inline void rotate() {
        auto anc = fa->fa;
        fa->pushdown(); pushdown();
        int tp = type(), ftp = fa->fa ? fa->type() : -1;
        fa->ch[tp] = ch[tp ^ 1]; if (ch[tp ^ 1]) ch[tp ^ 1]->fa = fa;
        this->ch[tp ^ 1] = fa, fa->fa = this;
        if (~ftp) anc->ch[ftp] = this; this->fa = anc;
        ch[tp ^ 1]->pushup(); pushup();
    }
    Node(int val = -0x7fffffff, Node *u = nullptr, Node *l = nullptr, Node *r = nullptr) :
        dat(val), fa(u), ch{l, r}, add(0), inv(false) { pushup(); }
} mempool[N];
static size_t counter;
template<typename... Args>
inline Node* alloc(Args... args) {
    mempool[counter] = std::move(Node(args...));
    return mempool + (counter++);
}
inline int id(Node *u) {
    return u ? u - mempool + 1 : 0;
}
}
struct Splay {
    using Node = SDM::Node;
    Node *root;
    inline void splay(Node *u, Node *goal = nullptr) {
        while (u->fa != goal) {
            if (u->fa->fa != goal)
                u->type() == u->fa->type() ? u->fa->rotate() : u->rotate();
            u->rotate();
        }
        if (!goal) root = u;
    }
    Splay(int n) {
        Node *u = SDM::alloc();
        for (int i = 1; i <= n; i++) {
            u->ch[1] = SDM::alloc(0, u);
            u = u->ch[1];
        }
        u->ch[1] = SDM::alloc(-0x7fffffff, u);
        splay(u->ch[1]);
    }
    inline Node* rank(int p) {
        ++p;
        Node *u = root;
        while (u->size > 1) {
            u->pushdown();
            if (p <= u->sz(0)) u = u->ch[0];
            else if (p <= u->size - u->sz(1)) return u;
            else p -= u->size - u->sz(1), u = u->ch[1];
        }
        return u;
    }
    inline Node* range(int l, int r) {
        // logprint("splay %d to root\n", SDM::id(rank(l)));
        splay(rank(l-1));
        // logprint("splay %d under root\n", SDM::id(rank(r)));
        splay(rank(r+1), root);
        root->ch[1]->pushdown();
        root->ch[1]->ch[0]->pushdown();
        return root->ch[1]->ch[0];
    }
    inline void dbg(int dep = 0) { dbg(root, dep); }
    inline void dbg(Node *u, int dep) {
        if (!u) return;
        u->pushdown();
        dbg(u->ch[0], dep + 2);
        for(int i=dep;i--;logprint(" ")); logprint("#%d dat=%d, sz=%d, mx=%d\n", SDM::id(u), u->dat, u->size, u->max);
        dbg(u->ch[1], dep + 2);
    }
};
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    Splay tr(n);
    // tr.dbg();
    for (int k, l, r, v; m--; ) {
        // logprint("================\n");
        scanf("%d%d%d", &k, &l, &r);
        if (k == 1) scanf("%d", &v);
        switch (k) {
        case 1: tr.range(l, r)->plus(v); break;
        case 2: tr.range(l, r)->invert(); break;
        case 3: printf("%d\n", tr.range(l, r)->max); break;
        default: assert(0);
        }
        // tr.dbg();
    }
    return 0;
}