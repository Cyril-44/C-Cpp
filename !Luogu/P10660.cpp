#include <stdio.h>
#include <algorithm>
constexpr int N = 30005, MOD = 10007;
namespace LCT {
#define U tr[u]
enum {L, R, FA};
struct Node {
    int ok, ob, k, b, sz, s[3];
    Node& operator()(int);
    int& operator[](int);
} tr[N]{1};
inline Node& Node::operator()(int x) { return tr[s[x]]; }
inline int& Node::operator[](int x) { return s[x]; }
inline bool type(int u) { return U(FA)[R] == u; }
inline bool isroot(int u) { return U(FA)[L] != u && U(FA)[R] != u; }
inline void pushup(int u) {
    U.sz = U(L).sz + 1 + U(R).sz;
    U.k = U.ok * U(L).k % MOD * U(R).k % MOD;
    U.b = (U.ob + U.ok * (U(L).b + U(R).b)) % MOD;
}
inline void rotate(int u) {
    bool tp = type(u); int anc = U(FA)[FA];
    if (isroot(U[FA])) U(FA)(FA)[type(U[FA])] = u;
    U(FA)[tp] = U[!tp]; if (U[!tp]) U(!tp)[FA] = u;
    U[!tp] = U[FA]; U(FA)[FA] = u; U[FA] = anc;
}
inline void splay(int u) {
    for (; !isroot(u); rotate(u))
        if (!isroot(U[FA]))
            rotate(type(u) == type(U[FA]) ? U[FA] : u);
}

#undef U
}
int main() {
    int n, q;
    scanf("%d", &n);
    for (int i = 1, k, p, b; i <= n; i++) {
        scanf("%d%d%d", &k, &p, &b);
        
    }
    scanf("%d", &q); char ch;
    for (int a, x, y, z, _q = 1; _q <= q; ++_q) {
        scanf( "%c%d", &ch, &a);
        if (ch == 'A') {

        } else {
            scanf("%d%d%d", &x, &y, &z);
        }
    }
    return 0;
}