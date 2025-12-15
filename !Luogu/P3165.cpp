#include <cassert>
#include <stdio.h>
#include <algorithm>
#include <string.h>
#define umin(x, y) ((x) < (y) ? (x) : (x) = (y))
constexpr int N = 100005, INF = 0x7fffffff;
int a[N];
#ifdef DEBUG
#define logprint(...) fprintf(stderr, __VA_ARGS__)
#else
#define logprint(...) ((void)0)
#endif
namespace SDM { // Splay Data Manager
static struct Node {
    Node *ch[2], *fa;
    int dat, min, size;
    bool inv;
    inline bool type() const { return fa->ch[1] == this; }
    inline int sz(bool c) const { return ch[c] ? ch[c]->size : 0; }
    inline int mn(bool c) const { return ch[c] ? ch[c]->min : INF; }
    inline void rotate() { // fa.fa ==> fa ==> this  ---->  fa.fa ==> this ==> fa
        fa->pushdown(); pushdown();
        auto anc = fa->fa;
        int tp = type();
        if (anc) anc->ch[fa->type()] = this;
        fa->ch[tp] = ch[tp ^ 1];
        if (ch[tp ^ 1]) ch[tp ^ 1]->fa = fa;
        ch[tp ^ 1] = fa; fa->fa = this; fa = anc;
        ch[tp ^ 1]->pushup(); pushup(); 
    }
    inline void pushup() {
        min = dat, size = 1;
        if (ch[0]) umin(min, ch[0]->min), size += ch[0]->size;
        if (ch[1]) umin(min, ch[1]->min), size += ch[1]->size;
    }
    inline void pushdown() {
        if (inv) {
            if (ch[0]) ch[0]->inv ^= 1;
            if (ch[1]) ch[1]->inv ^= 1;
            std::swap(ch[0], ch[1]);
            inv = false;
        }
    }
    inline void splay(Node *goal) {
        while (fa != goal) {
            if (fa->fa != goal)
                type() == fa->type() ? fa->rotate() : rotate();
            rotate();
        }
    }
    inline Node(int v = 0, Node *f = nullptr, Node *l = nullptr, Node *r = nullptr) :
        fa(f), dat(v), inv(false) { ch[0] = l, ch[1] = r; pushup(); }
} mempool[N];
static size_t counter = 0;
template<typename... Args>
inline Node* alloc(Args... args) {
    mempool[counter] = Node(args...);
    return mempool + (counter++);
}
inline int id(Node* u) { return u ? u - mempool + 1 : 0; }
}
struct Splay {
    using Node = SDM::Node;
    Node *root;
    inline Splay(int n) {
        static int _uni[N], _cnt[N];
        memcpy(_uni, a, sizeof a);
        std::sort(_uni+1, _uni+1 + n);
        for (int i = 1; i <= n; i++) {
            a[i] = std::lower_bound(_uni+1, _uni+1+n, a[i]) - _uni;
            a[i] += _cnt[a[i]]++;
        }
        Node *u = SDM::alloc(a[1]);
        for (int i = 2; i <= n; i++) {
            u->ch[1] = SDM::alloc(a[i], u);
            u = u->ch[1];
        } logprint("\n");
        splay(u);
    }
    inline void splay(Node* u, Node* goal = nullptr) {
        u->splay(goal);
        if (!goal) root = u;
    }
    inline Node* rank(int rk) {
        Node *u = root;
        while (u->size > 1) {
            if (rk <= u->sz(0)) u = u->ch[0];
            else if (rk <= u->size - u->sz(1)) return u;
            else rk -= u->size - u->sz(1), u = u->ch[1];
        }
        return u;
    }
    inline Node* range(int l, int r) {
        splay(rank(l-1)), splay(rank(r+1), root);
        return root->ch[1]->ch[0];
    }
    inline Node* first(Node *u) {
        while (u->pushdown(), u->ch[0]) u = u->ch[0];
        return u;
    }
    inline int sort_once() {
        Node *u = root;
        while (u->dat != u->min || u->dat == u->min && u->ch[0] && u->ch[0]->min == u->min) {
            u->pushdown();
            u = (u->mn(0) <= u->mn(1)) ? u->ch[0] : u->ch[1];
        }
        splay(u);
        // dbg();
        logprint("[ === #%d (v=%d, sz=%d, mn=%d)] splayed to root\n", SDM::id(u), u->dat, u->size, u->min);
        u->pushdown();
        int res = u->sz(0) + 1;
        if (u->ch[0]) u->ch[0]->inv ^= 1;
        if (u->ch[1]) splay(first(u->ch[1]), u);
        logprint("[ === #%d ] splayed under #%d\n", (dbg(), SDM::id(u->ch[1])), SDM::id(u));
        if (!u->ch[1]) root = u->ch[0];
        else {
            // assert(u->ch[1]->ch[0] == nullptr);
            u->ch[1]->ch[0] = u->ch[0];
            if (u->ch[0]) u->ch[0]->fa = u->ch[1];
            u->ch[1]->pushup();
            root = u->ch[1];
        }
        if (root) root->fa = nullptr;
        return res;
    }
    inline void dbg(int dep = 0) { dbg(root, dep); }
    inline void dbg(Node *u, int dep) {
        if (!u) return;
        u->pushdown();
        dbg(u->ch[0], dep + 2);
        for (int tp = dep; tp--; logprint(" "));
        logprint("Node#%d: (%d), mn=%d, sz=%d\n", SDM::id(u), u->dat, u->min, u->size);
        dbg(u->ch[1], dep + 2);
    }
};
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
    Splay tr(n);
    tr.dbg();
    for (int i = 1; i <= n; i++) {
        logprint("=========== %d ===========\n", i);
        printf("%d ", i - 1 + tr.sort_once());
#ifdef DEBUG
        tr.dbg();
#endif
    }
    putchar('\n');
    return 0;
}