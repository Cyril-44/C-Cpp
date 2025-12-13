#include <stdio.h>
#include <algorithm>
using LL = long long;
constexpr int N = 300004;
namespace Splay_Data {
struct Node {
    Node *ls, *rs, *fa;
    int size, cnt;
    long long val; // 区间的左边值
    bool inv;
    inline void pushup() {
        size = cnt;
        if (ls) size += ls->size;
        if (rs) size += rs->size;
    }
    inline void pushdown() {
        if (inv) {
            if (ls) ls->inv ^= 1;
            if (rs) rs->inv ^= 1;
            std::swap(ls, rs);
            inv = false;
        }
    }
    inline void zig() {
        pushdown(); auto u = rs; u->pushdown();
        auto anc = fa; if (anc) (anc->ls == this ? anc->ls : anc->rs) = u;
        u->fa = fa; fa = u; rs = u->ls;
        if (u->ls) u->ls->fa = this; u->ls = this;
    pushup(); u->pushup();
    }
    inline void zag() {
        pushdown(); auto v = ls; v->pushdown();
        auto anc = fa;  if (anc) (anc->ls == this ? anc->ls : anc->rs) = v;
        v->fa = fa; fa = v; ls = v->rs;
        if (v->rs) v->rs->fa = this; v->rs = this;
        pushup(); v->pushup();
    }
    inline void rotate() {
        (fa->ls == this) ? fa->zag() : fa->zig();
    }
};
inline Node* alloc() {
    static Node mempool[N * 20];
    static size_t ptr = 0;
    return mempool + (ptr++);
}
}
struct Splay_Dynamic {
using Node = Splay_Data::Node;
Node* root = nullptr;
/*
    fa           fa
    |     zig    |
    u    <====   v
   / \          / \
  v  tC  ====> tA  u
 / \      zag     / \
tA tB            tB tC
*/
void splay(Node* u) {splay(u, nullptr);}
void splay(Node* u, Node* tgt) { // 伸展 u 使 u->fa = tgt（默认旋到顶）
    while (u->fa != tgt) {
        // (O) ==l=> (P) ==r=> (U) 或 (O) ==r=> (P) ==l=> (u)：两次 rotate(u)
        // (O) ==l=> (P) ==l=> (U) 或 (O) ==r=> (P) ==r=> (u)：rotate(u->fa), rotate(u)
        if (u->fa->fa != tgt) {
            (u->fa->ls == u) == (u->fa->fa->ls == u->fa) ? u->fa->rotate() : u->rotate();
        }
        u->rotate();
    }
    if (!tgt) root = u; // 旋到根了
}
inline void init(int n, LL v) {
    root = Splay_Data::alloc();
    root->cnt = n, root->val = v;
    root->pushup();
}
inline Node* rank(int rk) {
    ++rk;
    auto u = root;
    while (u->size > 1) {
        u->pushdown();
        int lsz = u->ls ? u->ls->size : 0;
        int rsz = u->rs ? u->rs->size : 0;
        if (lsz < rk && rk <= u->size - rsz) break;
        if (rk <= lsz) u = u->ls;
        else rk -= u->size - rsz, u = u->rs;
    }
    // printf("Find #%d %d\n", u->dat, u->size);
    return u;
}
inline Node* pre(Node* u) {
    
}
inline void erase(Node* u) {
    if (u->cnt == 1) {
        
    }
}
inline void push_back(LL x) {
    if (!root) {
        root = Splay_Data::alloc();
        root->val = x;
        root->cnt = 1;
        root->pushup();
        return;
    }
    Node* curr = root;
    while (curr->rs) curr = curr->rs;
    curr->rs = Splay_Data::alloc();
    curr->rs->fa = curr;
    curr = curr->fa;
    curr->val = x;
    curr->cnt = 1;
    curr->pushup();
    splay(curr);
}
inline void dbg(Node *u, int dep) {
    if (!u) return;
    if (u->ls) {
    for (int i = dep; i--; ) putchar(' '); puts("Left:");
    dbg(u->ls, dep + 4); }
    for (int i = dep; i--; ) putchar(' ');
    printf("#%d sz=%d\n", u->cnt, u->size);
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
} last_col, row[N];
int main() {
    int n, m, q;
    scanf("%d%d%d", &n, &m, &q);
    for (int x, y; q--;) {
        scanf("%d%d", &x, &y);
    }
}