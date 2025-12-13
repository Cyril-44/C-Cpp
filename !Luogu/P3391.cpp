#include <stdio.h>
#include <string.h>
#include <algorithm>
constexpr int N = 100005;
namespace Splay {
struct Node {
    Node *ls, *rs, *fa;
    int size, dat, cnt;
    bool inv;
} *root;
inline void pushup(Node* u) {
    u->size = u->cnt;
    if (u->ls) u->size += u->ls->size;
    if (u->rs) u->size += u->rs->size;
}
inline void pushdown(Node *u) {
    if (u->inv) {
        if (u->ls) u->ls->inv ^= 1;
        if (u->rs) u->rs->inv ^= 1;
        std::swap(u->ls, u->rs);
        u->inv = false;
    }
}
inline Node* alloc() {
    static Node mempool[N];
    static size_t ptr = 0;
    return mempool + (ptr++);
}
/*
    fa           fa
    |     zig    |
    u    <====   v
   / \          / \
  v  tC  ====> tA  u
 / \      zag     / \
tA tB            tB tC
*/
inline void zig(Node* v) {
    // puts("Zigged");
    pushdown(v);
    auto u = v->rs;
    pushdown(u);
    auto anc = v->fa;
    u->fa = v->fa;
    v->fa = u;
    if (anc) (anc->ls == v ? anc->ls : anc->rs) = u;
    v->rs = u->ls;
    if (u->ls) u->ls->fa = v;
    u->ls = v;
    pushup(v);
    pushup(u);
}
inline void zag(Node* u) {
    // puts("Zagged");
    pushdown(u);
    auto v = u->ls;
    pushdown(v);
    auto anc = u->fa;
    v->fa = u->fa;
    u->fa = v;
    if (anc) (anc->ls == u ? anc->ls : anc->rs) = v;
    u->ls = v->rs;
    if (v->rs) v->rs->fa = u;
    v->rs = u;
    pushup(u);
    pushup(v);
}
void rotate(Node* u) {
    (u->fa->ls == u) ? zag(u->fa) : zig(u->fa);
}
void splay(Node* u, Node* tgt = nullptr) { // 伸展 u 使 u->fa = tgt（默认旋到顶）
    while (u->fa != tgt) {
        // (O) ==l=> (P) ==r=> (U) 或 (O) ==r=> (P) ==l=> (u)：两次 rotate(u)
        // (O) ==l=> (P) ==l=> (U) 或 (O) ==r=> (P) ==r=> (u)：rotate(u->fa), rotate(u)
        if (u->fa->fa != tgt) {
            (u->fa->ls == u) == (u->fa->fa->ls == u->fa) ? rotate(u->fa) : rotate(u);
        }
        rotate(u);
    }
    if (!tgt) root = u; // 旋到根了
}
inline void build(int n) {
    auto u = alloc();
    u->cnt = 1;
    for (int i = 1; i <= n; i++) {
        u->rs = alloc();
        u->rs->fa = u;
        u = u->rs;
        u->dat = i;
        u->cnt = 1;
    }
    auto guard = alloc();
    guard->dat = 0x7fffffff;
    guard->fa = u;
    guard->cnt = 1;
    u->rs = guard;
    splay(guard); // 维护 size
}
inline Node* rank(int rk) {
    ++rk;
    auto u = root;
    while (u->size > 1) {
        pushdown(u);
        int lsz = u->ls ? u->ls->size : 0;
        int rsz = u->rs ? u->rs->size : 0;
        if (lsz < rk && rk <= u->size - rsz) break;
        if (rk <= lsz) u = u->ls;
        else rk -= u->size - rsz, u = u->rs;
    }
    // printf("Find #%d %d\n", u->dat, u->size);
    return u;
}
inline void dbg(Node *u = root, int dep = 0) {
    if (!u) return;
    if (u->ls) {
    for (int i = dep; i--; ) putchar(' '); puts("Left:");
    dbg(u->ls, dep + 4); }
    for (int i = dep; i--; ) putchar(' ');
    printf("#%d sz=%d\n", u->dat, u->size);
    if (u->rs) {
    for (int i = dep; i--; ) putchar(' '); puts("Right:");
    dbg(u->rs, dep + 4); }
}
inline Node* range(int l, int r) {
    splay(rank(l-1));
    // dbg(rank(r+1));
    splay(rank(r+1), root);
    // dbg();
    return root->rs->ls; // 对应区间 [l, r]
}
}
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    Splay::build(n);
    for (int l, r; m--; ) {
        scanf("%d%d", &l, &r);
        auto pts = Splay::range(l, r);
        pts->inv ^= 1;
        // Splay::dbg();
    }
    for (int i = 1; i <= n; i++)
        printf("%d ", Splay::rank(i)->dat);
    putchar('\n');
    return 0;
}