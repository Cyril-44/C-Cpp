#include <stdio.h>
#include <algorithm>
constexpr int N = 100005;
int a[N];
enum PointerType {L, R, FA};
struct SplayNode {
    int s[3], sz, val;
    bool inv;
    SplayNode& go(int);
    inline void reverse() { std::swap(s[L], s[R]); inv ^= 1; }
    inline void pushup() { sz = go(L).sz + 1 + go(R).sz; }
    inline void pushdown() {
        if (inv) {
            if (s[L]) go(L).reverse();
            if (s[R]) go(R).reverse();
            inv = false;
        }
    }
} tr[N];
inline bool type(int u) { return tr[u].go(FA).s[R] == u; }
inline void rotate(int u) {
    int anc = tr[u].go(FA).s[FA];
    bool tp = type(u), ftp = type(tr[u].s[FA]);
    auto &i = tr[u];
    i.go(FA).s[tp] = i.s[!tp]; if (i.s[!tp]) i.go(!tp).s[FA] = tr[u].s[FA];
    i.s[!tp] = i.s[FA]; i.go(FA).s[FA] = u;
    {if (anc) tr[anc].s[ftp] = u;} i.s[FA] = anc;
}
inline void splay(int u, int v = 0) {
    while (tr[u].s[FA] ^ v) {
        if (tr[u].go(FA).s[FA] != v)
            type(u) == type(tr[u].s[FA]) ? rotate(tr[u].s[FA]) : rotate(u);
        rotate(u);
    }
}
inline SplayNode& SplayNode::go(int tp) { return tr[s[tp]]; };
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    return 0;
}