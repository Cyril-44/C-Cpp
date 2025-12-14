#include <stdio.h>
#include <algorithm>
using LL = long long;
constexpr int N = 300004;
namespace Splay_Data {
struct Node {
    Node *ls, *rs, *fa;
    int size, cnt;
    LL val; // 区间的左边值
    inline Node(Node* l=nullptr, Node* r=nullptr, Node* f=nullptr, int c=0, LL v=0) : 
        ls(l), rs(r), fa(f), size(0), cnt(c), val(v) {}
    inline void pushup() {
        size = cnt;
        if (ls) size += ls->size;
        if (rs) size += rs->size;
    }
    inline void zig() {
        auto u = rs;
        auto anc = fa; if (anc) (anc->ls == this ? anc->ls : anc->rs) = u;
        u->fa = fa; fa = u; rs = u->ls;
        if (u->ls) u->ls->fa = this; u->ls = this;
        pushup(); u->pushup();
    }
    inline void zag() {
        auto v = ls;
        auto anc = fa;  if (anc) (anc->ls == this ? anc->ls : anc->rs) = v;
        v->fa = fa; fa = v; ls = v->rs;
        if (v->rs) v->rs->fa = this; v->rs = this;
        pushup(); v->pushup();
    }
    inline void rotate() {
        (fa->ls == this) ? fa->zag() : fa->zig();
    }
    inline int lsz() const { return ls ? ls->size : 0; }
    inline int rsz() const { return rs ? rs->size : 0; }
};
inline Node* alloc() {
    static Node mempool[N * 20];
    static size_t ptr = 0;
    return mempool + (ptr++);
}
template<typename... Args>
inline Node* alloc(Args... args) {
    Node* res = alloc();
    *res = Node(args...);
    return res;
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
        if (u->fa->fa != tgt)
            (u->fa->ls == u) == (u->fa->fa->ls == u->fa) ? u->fa->rotate() : u->rotate();
        u->rotate();
    }
    if (!tgt) root = u; // 旋到根了
}
inline void init(int n, LL v) {
    root = Splay_Data::alloc(nullptr, nullptr, nullptr, n, v);
    root->pushup();
}
inline Node* rank(int rk) {
    auto u = root;
    while (u->ls || u->rs) {
        if (u->lsz() < rk && rk <= u->size - u->rsz()) break;
        if (rk <= u->lsz()) u = u->ls;
        else rk -= u->size - u->rsz(), u = u->rs;
    }
    // printf("Find #%d %d\n", u->dat, u->size);
    return u;
}
inline Node* last(Node* u) { // 找 u 子树中的最大节点
    while (u->rs) u = u->rs;
    return u;
}
inline Node* pre(Node* u) {
    return u->ls ? last(u->ls) : u->fa;
}
inline void erase() {
    if (root->ls) {
        splay(last(root->ls), root);
        root->ls->rs = root->rs;
        root->rs->fa = root->ls;
        root = root->ls;
    } else if (root->rs)
        root = root->rs;
    else return root = nullptr, void();
    root->fa = nullptr;
}
inline LL erase(int p) {
    Node *u = rank(p);
    splay(u);
    if (u->cnt == 1) return erase(), u->val;
    if (u->lsz() + 1 == p) { // p 在最左边
        Node *restr = Splay_Data::alloc(nullptr, u->rs, u, u->cnt - 1, u->val + 1);
        u->cnt = 1, u->rs = restr;
        return erase(), u->val;
    }
    if (u->lsz() + u->cnt - 1 == p) { // p 在最右边
        Node *restl = Splay_Data::alloc(u->ls, nullptr, u, u->cnt - 1, u->val);
        u->ls = restl, u->val += u->cnt - 1, u->cnt = 1;
        return erase(), u->val;
    }
    Node *restl = Splay_Data::alloc(u->ls, nullptr, u, p - u->lsz() - 1, u->val),
         *restr = Splay_Data::alloc(nullptr, u->rs, u, u->lsz() + u->cnt - 1 - p, u->val + p - u->lsz() + 1);
    u->cnt = 1, u->val += p - u->lsz(), u->ls = restl, u->rs = restr;
    return erase(), u->val;
}
inline void push_back(LL x) {
    if (!root) { // Specific: when empty
        root = Splay_Data::alloc();
        root->val = x;
        root->cnt = 1;
        root->pushup();
        return;
    }
    Node* curr = last(root);
    curr->rs = Splay_Data::alloc(nullptr, nullptr, curr, 1, x);
    splay(curr->rs);
}
inline void dbg(Node *u, int dep) {
    if (!u) return;
    dbg(u->ls, dep + 2);
    for (int i = dep; i--; ) putchar(' ');
    printf("[%lld,%lld] sz=%d\n", u->val, u->val + u->cnt-1, u->size);
    dbg(u->rs, dep + 2);
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
    for (int i = 1; i <= n; i++) {
        row[i].init(m - 1, (i-1ll) * m + 1);
        last_col.push_back(i * m);
    }
        /* puts("Last column: ");
        last_col.dbg(last_col.root, 2);
        for (int i = 1; i <= n; i++) {
            printf("Row %d: \n", i);
            row[i].dbg(row[i].root, 4);
        } */
    for (int x, y; q--;) {
        scanf("%d%d", &x, &y);
        LL lc = last_col.erase(x);
        if (y != m) {
            LL mr = row[x].erase(y);
            row[x].push_back(lc);
            last_col.push_back(mr);
            printf("%lld\n", mr);
        } else {
            printf("%lld\n", lc);
            last_col.push_back(lc);
        }
        /* puts("Last column: ");
        last_col.dbg(last_col.root, 2);
        for (int i = 1; i <= n; i++) {
            printf("Row %d: \n", i);
            row[i].dbg(row[i].root, 4);
        } */
    }
}