#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <assert.h>
constexpr int N = 30005, MOD = 10007;
int p[N];
inline int qpow(int b, int n = MOD - 2) {
    int res = 1;
    while (n) {
        if (n & 1) res = res * b % MOD;
        b = b * b % MOD;
        n >>= 1;
    }
    return res;
}
namespace LCT {
#define U tr[u]
enum {L, R, FA};
/* dat 表示当前节点可以由 k * fa.val + b 得到 
 * 这里的 AuxTree 维护的一段偏爱链，sum 计算的是 当前节点对应的链底 = k * 链顶 + b。
 * e.g. 如果维护了一段偏爱链 p_{1 \dots k} 那么 root 的 sum 就表示 pn = k * p1 + b
 */
struct Node {
    struct Statistics {
        int k, b;
        inline Statistics operator*(const Statistics& o) const {
            return Statistics{k * o.k % MOD, (o.k * b + o.b) % MOD};
        }
        inline Statistics operator~() const {
            return Statistics{qpow(k), (MOD-b) * k % MOD};
        }
        inline int operator*(int x) const { return (k * x + b) % MOD; }
    } dat, prod;
    int s[3];
    bool inv;
    Node& operator()(int);
    inline int& operator[](int x) { return s[x]; }
} tr[N]{1,0,1,0,0,0,0,0,false};
int sol[N]; // 可以发现，如果可以提取出一条以已知解为链底的偏爱链，那么就可以直接计算了
int extra[N]; // 记录基环树上多出来的那一条边（这一条边一定连接在 root 上，且这颗树在被 cut 之前不可能再有 link 和 changeroot 操作
inline Node& Node::operator()(int x) { return tr[s[x]]; }
inline bool type(int u) { return U(FA)[R] == u; }
inline bool isroot(int u) { return U(FA)[L] != u && U(FA)[R] != u; }
inline void pushup(int u) { U.prod = U(L).prod * U.dat * U(R).prod; }
inline void reverse(int u) {
    std::swap(U[L], U[R]);
    U.inv ^= 1;
    pushup(u);
}
inline void pushdown(int u) {
    if (U.inv) {
        if (U[L]) reverse(U[L]);
        if (U[R]) reverse(U[R]);
        U.inv = false;
    }
}
void sync(int u) { if (!isroot(u)) sync(U[FA]); pushdown(u); }
inline void rotate(int u) {
    bool tp = type(u);
    int fa = U[FA], anc = U(FA)[FA];
    if (!isroot(fa)) tr[anc][type(fa)] = u;
    tr[fa][tp] = U[!tp]; if (U[!tp]) U(!tp)[FA] = fa;
    U[!tp] = fa; tr[fa][FA] = u; U[FA] = anc;
    pushup(fa), pushup(u);
}
inline void splay(int u) {
    sync(u);
    for (; !isroot(u); rotate(u))
        if (!isroot(U[FA]))
            rotate(type(u) == type(U[FA]) ? U[FA] : u);
}
inline int access(int u) {
    int p = 0;
    for (; u; p = u, u = U[FA])
        splay(u), U[R] = p, pushup(u);
    return p;
}
inline int getroot(int u) {
    for (u = access(u); U[L]; u = U[L]) pushdown(u);
    return splay(u), u;
}
inline void changeroot(int u) { access(u), splay(u), reverse(u); }

namespace Functions {
inline void edit(int u, int k, int b) { splay(u); U.dat = {k, b}; pushup(u); }
inline void link(int u, int v) { // u -> v, u.k * v.val + u.b = u.val
    if (getroot(u) != getroot(v)) {
        changeroot(u), U[FA] = v;
        // fprintf(stderr, "%d --> %d\n", u, v);
    }
    else { // 准备解方程。
        changeroot(u); access(v);
        extra[u] = v;
        // 此时我们得到了 u <-- v <- u 的一个环，那么就有 au * prodk + prodb = au
        // 解方程，可以得到 au * (prodk - 1) = -prodb
        auto s = U(R).prod * U.dat; // u -> v 存储在 U.dat 中
        int k = (s.k - 1 + MOD) % MOD, b = (MOD - s.b) % MOD;
        sol[u] = k ? (b * qpow(k) % MOD) // sol = b / k;
                   : (b ? -1 : -2);      // b!=0 无解, b=0 无数组解
        // fprintf(stderr, "Solved #%d: %d * \033[4m%d\033[0m = %d\n", u, k, sol[u], b);
    }
}
inline void cut(int u, int v) { // Remove u -> v
    int exu = getroot(v), exv = extra[exu];
    extra[exu] = 0; sol[exu] = -1;
    if (u == exu && v == exv) return;
    changeroot(v); access(u); splay(v);
    assert(tr[v][L] == u); // 此时偏爱链应该只有 u -> v
    tr[v][L] = U[FA] = 0;
    pushup(v);
    if (exv) link(exu, exv);
}
inline int query(int u) {
    int rt = getroot(u);
    if (sol[rt] < 0) return sol[rt];
    access(u); splay(rt); // 得到了一条 rt <- u 的链
    return tr[rt](R).prod * sol[rt];
}
void print(int u, int dep = 0) {
    if (!u) return;
    pushdown(u);
    print(U[L], dep + 2);
    for (int i = dep; i--; putchar(' '));
    printf("%d [FA=%d]: k,b = (%d,%d) {%d,%d}\n", u, U[FA], U.dat.k, U.dat.b, U.prod.k, U.prod.b);
    print(U[R], dep + 2);
}
}
#undef U
void* __init_sol = []() { return memset(sol, -1, sizeof sol); } (); // 初始全部无解
} 
using namespace LCT::Functions;

int main() {
    // auto tmp = ((LCT::Node::Statistics){2, 4} * (LCT::Node::Statistics){2, 3} * LCT::tr[0].prod);
    // printf("%d, %d\n", tmp.k, tmp.b);

    int n, q;
    scanf("%d", &n);
    for (int i = 1, k, b; i <= n; i++) {
        scanf("%d%d%d", &LCT::tr[i].dat.k, &p[i], &LCT::tr[i].dat.b);
        // printf("%d: k=%d, b=%d, (%d,%d)\n", i, LCT::tr[i].dat.k, LCT::tr[i].dat.b, LCT::tr[i].prod.k, LCT::tr[i].prod.b);
    }
    for (int i = 1; i <= n; i++) {
        link(i, p[i]);
        // puts("=========="); print(i); puts("----------"); print(p[i]);
    }
    // puts("========"); print(5); puts("========");
    scanf("%d", &q); char ch;
    for (int a, x, y, z, _q = 1; _q <= q; ++_q) {
        scanf(" %c%d", &ch, &a);
        if (ch == 'A') {
            printf("%d\n", query(a));
            // print(5);
        } else {
            scanf("%d%d%d", &x, &y, &z);
            cut(a, p[a]);
            edit(a, x, z);
            link(a, p[a] = y);
        }
    }
    return 0;
}