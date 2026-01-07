#include <filesystem>
#include <stdio.h>
#include <algorithm>
#include <array>
constexpr int N = 100005;
using ULL = unsigned long long;
using Oper = std::array<ULL, 2>;
#ifdef DEBUG
#define debug printf
#else
#define debug(...) ((void)0)
#endif
namespace LCT {
#define U tr[u]
enum { L, R, FA };
struct Node {
    std::array<int, 3> s;
    Oper op, prod, rprod;
    bool rev;
    friend int getid(Node*);
    inline void reverse() {
        debug(" ==>| Reverse %d: %d <-> %d! rev=%d\n", getid(this), s[L], s[R], (int)rev);
        rev ^= 1;
        std::swap(s[L], s[R]);
        std::swap(prod, rprod);
    }
    Node() : prod{0ull, ~0ull}, rprod{0ull, ~0ull}, rev(false) {}
    inline int& operator[](int x) { return s[x]; }
    Node& operator()(int x);
} tr[N];
inline int getid(Node* ptr) { return ptr - tr; }
inline Node& Node::operator()(int x) { return tr[s[x]]; }
inline Oper operator*(const Oper& x, const Oper& y) {
    return Oper{x[0] & y[1] | ~x[0] & y[0], x[1] & y[1] | ~x[1] & y[0]};
}
inline bool type(int u) { return U(FA)[R] == u; }
inline bool isroot(int u) { return U(FA)[L] != u && U(FA)[R] != u; }
inline void pushup(int u) {
    U.prod = U(L).prod * U.op * U(R).prod;
    U.rprod = U(R).rprod * U.op * U(L).rprod;
}
inline void pushdown(int u) {
    if (!U.rev) return;
    if (U[L]) U(L).reverse();
    if (U[R]) U(R).reverse();
    U.rev = false;
}
void sync(int u) {
    if (!isroot(u)) sync(U[FA]);
    pushdown(u);
    debug(" ==>| Pushdown %d! (fa=%d, %d, %d, %d)\n", u, U[FA], (int)(isroot(u)), U(FA)[L], U(FA)[R]);
}
inline void rotate(int u) {
    pushdown(U[FA]), pushdown(u);
    debug("%s rotated %d\n", type(u) ? "Left" : "Right", u);
    bool tp = type(u); int anc = U(FA)[FA];
    if (!isroot(U[FA])) U(FA)(FA)[type(U[FA])] = u, debug("not root set %d.fa=%d\n", U(FA)[FA], u);
    else debug("%d is root! %d,%d\n", U[FA], U(FA)(FA)[L], U(FA)(FA)[R]);
    debug("%d.%c=%d, %d.fa=%d\n", U[FA], tp ? 'R' : 'L', U[!tp], U[!tp], U[FA]);
    U(FA)[tp] = U[!tp]; if (U[!tp]) U(!tp)[FA] = U[FA];
    debug("%d.%c=%d, %d.fa=%d, %d.fa=%d\n", u, tp?'L':'R', U[FA], U[FA], u, u, anc);
    U[!tp] = U[FA]; U(FA)[FA] = u; U[FA] = anc;
    pushup(U[!tp]), pushup(u);
}
void print(int u, int dep) {
    if (!u) return;
    // pushdown(u);
    print(U[L], dep + 2);
    for (int _=dep;_--;putchar(' '));
    printf("%d l=%d, r=%d, fa=%d %s\n", u, U[L], U[R], U[FA], U.rev ? "(rev)" : "");
    print(U[R], dep + 2);
}
inline void splay(int u) {
    // debug("Before (\n"), print(4, 2), debug(")\n");
    sync(u);
    // debug("After (\n"), print(4, 2), debug(")\n");
    for (; !isroot(u); rotate(u))
        if (!isroot(U[FA]))
            rotate(type(u) == type(U[FA]) ? U[FA] : u);
}
inline int access(int u) {
    int p = 0;
    for (; u; p = u, u = U[FA]) {
        splay(u), U[R] = p, pushup(u);
        debug("=== Rotated %d to its root, make %d connected. ===\n", u, p);
        // print(u, 4);
    }
    return p;
}
inline void makeroot(int u) { debug("[\n"); tr[access(u)].reverse(); debug("]\n"); }
namespace Utils {
inline void link(int u, int v) {
    makeroot(u); // print(4, 3);
    splay(u);
    // print(u, 2);
    U[FA] = v;
}
inline void modify(int u, int op, ULL val) {
    splay(u);
    switch (op) {
    case 1: U.op = {0ull, val}; break;
    case 2: U.op = {val, ~0ull}; break;
    case 3: U.op = {val, ~val}; break;
    }
    pushup(u);
}
inline Oper query(int u, int v) {
    makeroot(u);
    return tr[access(v)].prod;
}
}
#undef U
}
using namespace LCT::Utils;
int main() {
    int n, m, k, x, y; ULL z;
    scanf("%d%d%d", &n, &m, &k);
    for (int i = 1; i <= n; i++) {
        scanf("%d%llu", &x, &z);
        modify(i, x, z);
    }
    for (int i = 1; i < n; i++) {
        scanf("%d%d", &x, &y);
        link(x, y);
        // for (int jjj = 1; jjj <= n; jjj++)
        //     printf("\t%d: fa=%d, l r=%d %d, op=(%llX,%llX), prod=(%llX,%llX) %s\n", jjj, LCT::tr[jjj][2], LCT::tr[jjj][0], LCT::tr[jjj][1], LCT::tr[jjj].op[0], LCT::tr[jjj].op[1], LCT::tr[jjj].prod[0], LCT::tr[jjj].prod[1], LCT::tr[jjj].rev ? "(rev)" : "");
        // printf("======\n");
    }

    for (int _q = 1, q; _q <= m; _q++) {
        scanf("%d%d%d%llu", &q, &x, &y, &z);
        if (q == 1) {
            auto res = query(x, y);
            ULL ans = z & res[1] | ~z & res[0], tp1 = res[0] | res[1], tp2 = ans;
            ULL m1 = 0ull, m2 = ~0ull;
            for (int dg = 0; dg < 64; m1 |= 1ull << dg++) {
                m2 ^= 1ull << dg;
                if (z >> dg & 1) {
                    ans = std::max(ans, tp1 & m1 | tp2 & m2 | res[0] & 1ull << dg);
                }
            }
            printf("%llu\n", ans);
        } else {
            modify(x, y, z);
        }
        // printf("%s %d:\n", q-1?"Modify":"Query", _q);
        // for (int i = 1; i <= n; i++)
        //     printf("\t%d: fa=%d, l r=%d %d, op=(%llX,%llX), prod=(%llX,%llX) %s\n", i, LCT::tr[i][2], LCT::tr[i][0], LCT::tr[i][1], LCT::tr[i].op[0], LCT::tr[i].op[1], LCT::tr[i].prod[0], LCT::tr[i].prod[1], LCT::tr[i].rev ? "(rev)" : "");
    }
    return 0;
}