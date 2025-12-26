#include <stdio.h>
#include <algorithm>
constexpr int N = 30005, MOD = 10007;
struct { int k, p, b; } a[N];
namespace LCT {
#define U tr[u]
enum {L, R, FA};
struct Node {
    int ok, ob, sol, k, b, sz, s[3];
    bool inv;
    Node& operator()(int);
    inline int& operator[](int x) { return s[x]; }
    inline void reverse() {
        std::swap(s[L], s[R]);
        inv ^= 1;
    }
} tr[N]{1};
inline Node& Node::operator()(int x) { return tr[s[x]]; }
inline bool type(int u) { return U(FA)[R] == u; }
inline bool isroot(int u) { return U(FA)[L] != u && U(FA)[R] != u; }
inline void pushup(int u) {
    U.sz = U(L).sz + 1 + U(R).sz;
    U.k = U.ok * U(L).k % MOD * U(R).k % MOD;
    U.b = (U.ob + U.ok * (U(L).b + U(R).b)) % MOD;
}
inline void pushdown(int u) {
    if (U.inv) {
        if (U[L]) U(L).reverse();
        if (U[R]) U(R).reverse();
        U.inv = false;
    }
}
void sync(int u) { if (U[FA]) sync(U[FA]); pushdown(u); }
inline void rotate(int u) {
    bool tp = type(u); int anc = U(FA)[FA];
    if (isroot(U[FA])) U(FA)(FA)[type(U[FA])] = u;
    U(FA)[tp] = U[!tp]; if (U[!tp]) U(!tp)[FA] = u;
    U[!tp] = U[FA]; U(FA)[FA] = u; U[FA] = anc;
    pushup(U[!tp]), pushup(u);
}
inline void splay(int u) {
    sync(u);
    for (; !isroot(u); rotate(u))
        if (!isroot(U[FA]))
            rotate(type(u) == type(U[FA]) ? U[FA] : u);
}
inline void access(int u) {
    int p = 0;
    for (; u; p = u, u = U[FA])
        splay(u), U[R] = p, pushup(u);
}
inline void changeroot(int u) { access(u), splay(u), U.reverse(); }
namespace Functions {
inline void edit(int u, int k, int b) { U.ok = k, U.ob = b; pushup(u); }
inline void link(int u, int v) { // u -> v
    changeroot(u), U[FA] = v;
}
inline void cut(int u, int v) { // Remove u -> v
    changeroot(u); splay(v);
    U[R] = tr[v][FA] = 0;
}
}
#undef U
}
using namespace LCT::Functions;
int main() {
    int n, q;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%d%d%d", &a[i].k, &a[i].p, &a[i].b);
        edit(i, a[i].k, a[i].b);
    }
    for (int i = 1; i <= n; i++) link(i, a[i].p);
    scanf("%d", &q); char ch;
    for (int a, x, y, z, _q = 1; _q <= q; ++_q) {
        scanf( "%c%d", &ch, &a);
        if (ch == 'A') {
            x = 1;
        } else {
            scanf("%d%d%d", &x, &y, &z);
        }
    }
    return 0;
}