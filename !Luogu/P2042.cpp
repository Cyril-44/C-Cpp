#include <stdio.h>
#include <string.h>
#include <algorithm>
constexpr int N = 500005, INF = 1 << 31;
namespace SDM {
static struct Node {
    Node *ch[2], *fa;
    int dat, sz;
    int mx, premx, sufmx, sm;
    bool inv;
#define DECL(func, var, fallback) inline int func(bool c) { return ch[c] ? ch[c]->var : fallback; }
    DECL(max, mx, INF) DECL(pre, premx, INF) DECL(suf, sufmx, INF) DECL(sum, sm, 0) DECL(size, sz, 0)
#undef DECL
    inline bool type() const { return fa->ch[1] == this; }
    inline void pushdown() {
        if (inv) {
            if (ch[0]) ch[0]->inv ^= 1;
            if (ch[1]) ch[1]->inv ^= 1;
        }
    }
    inline void pushup() {
        premx = pre(0) == max(0) ? pre(0) + pre(1) : pre(0);
        sufmx = suf(1) == max(1) ? suf(0) + suf(1) : suf(1);
        mx = suf(0) + pre(1);
        sm = sum(0) + sum(1);
        sz = size(0) + 1 + size(1);
    }
    inline Node(Node* f = nullptr, int v = INF, Node *l = nullptr, Node *r = nullptr):
        ch{l, r}, fa(f), dat(v), sz(1), mx(INF), premx(INF), sufmx(INF), sm(0), inv(false) {}
    inline Node* begin() { Node *u = this; while (u->pushdown(), u->ch[0]) u = u->ch[0]; return u; }
    inline Node* end() { Node *u = this; while (u->pushdown(), u->ch[1]) u = u->ch[1]; return u; }
    inline void rotate() {
        fa->pushdown(), pushdown();
        auto anc = fa->fa;
        bool tp = type(), ftp = fa->type();
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
        for (Node *u = root; u->sz > 1; ) {
            u->pushdown();
            if (p <= u->size(0)) u = u->ch[0];
            else if (p <= u->sz - u->size(1)) return u;
            else u = u->ch[1];
        }
        throw "Fuck you";
    }
    inline Node* range(int l, int r) {
        splay(rank(l-1)), splay(rank(r+1), root);
        return root->ch[1]->ch[0];
    }
    inline void insert(int p, int *a, int n) {
        Node *rt = rank(p);
        splay(rt->ch[1]->begin(), rt);
        Node *u = rt->ch[1];
        for (int i = n; i >= 1; i--) {
            u->ch[0] = SDM::alloc(u, a[i]);
            u = u->ch[0];
        }
        splay(u, rt);
    }
};
int main() {
    return 0;
}