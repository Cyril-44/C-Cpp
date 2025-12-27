#include <stdio.h>
#include <string.h>
#include <algorithm>
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
        inline Statistics operator+(const Statistics& o) const {
            return Statistics{k * o.k % MOD, (k * o.b + b) % MOD};
        }
        inline Statistics operator~() const { Statistics a = *this; a.reverse(); return a; }
        inline int operator*(int x) const { return k * x + b % MOD; }
        inline void reverse() { // 如果将 a1..an 翻转，那么原先的 an = k * a1 + b ==> a1 = 1/k * an - b/k
            k = qpow(k);
            b = (MOD-b) * k % MOD;
        }
    } dat, sum;
    int sz, s[3];
    bool inv;
    Node& operator()(int);
    inline int& operator[](int x) { return s[x]; }
} tr[N]{1,0,1,0,0,0,0,0,false};
int sol[N]; // 可以发现，如果可以提取出一条以已知解为链底的偏爱链，那么就可以直接计算了
int extra[N]; // 记录基环树上多出来的那一条边（这一条边一定连接在 root 上，且这颗树在被 cut 之前不可能再有 link 和 changeroot 操作
inline Node& Node::operator()(int x) { return tr[s[x]]; }
inline bool type(int u) { return U(FA)[R] == u; }
inline bool isroot(int u) { return U(FA)[L] != u && U(FA)[R] != u; }
inline void pushup(int u) {
    U.sz = U(L).sz + 1 + U(R).sz;
    U.sum = U(L).sum + U.dat + U(R).sum;
}
inline void reverse(int u) {
    std::swap(U[L], U[R]);
    U.inv ^= 1;
    U.dat.reverse();
    pushup(u);
}
inline void pushdown(int u) {
    if (U.inv) {
        if (U[L]) reverse(U[L]);
        if (U[R]) reverse(U[R]);
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
inline int access(int u) {
    int p = 0;
    for (; u; p = u, u = U[FA])
        splay(u), U[R] = p, pushup(u);
    return p;
}
inline int getroot(int u) {
    u = access(u);
    while (U[L]) pushdown(u), u = U[L];
    splay(u);
    return u;
}
inline void changeroot(int u) {
    int rt = getroot(u);
    access(u), splay(u), reverse(u);
    sol[u] = sol[rt] < 0 ? sol[rt] : ~tr[access(rt)].sum * sol[rt]; // 注意判断无解
}

namespace Functions {
inline void edit(int u, int k, int b) { U.dat = {k, b}; pushup(u); }
inline void link(int u, int v) { // u -> v, u.k * v.val + u.b = u.val
    if (getroot(u) != getroot(v))
        changeroot(u), U[FA] = v;
    else { // 准备解方程。
        changeroot(u); access(v); splay(v); splay(u);
        extra[u] = v;
        // 此时我们得到了 u --> v 的一条链，那么就有 au * sumk + sumb = av，且我们还有 au * u.k + u.b = av
        // 解方程，可以得到 au * (k1 - k2) + b1 - b2 = 0
        auto s1 = U.dat; U.dat = {1, 0}; pushup(u);
        auto s2 = U.sum;
        int k = s1.k - s2.k, b = s2.b - s1.b;
        if (k) sol[u] = b * qpow(k) % MOD; // sol = b / k;
        else sol[u] = b ? -1 : -2; // b!=0 无解, b=0 无数组解
    }
}
inline void cut(int u, int v) { // Remove u -> v
    int exu = getroot(u), exv = extra[exu];
    extra[exu] = 0;
    changeroot(v); access(u); splay(u);
    U[R] = tr[v][FA] = 0; pushup(u);
    if (exv) link(exu, exv);
}
inline int query(int u) {
    access(u); splay(u);
    int rt = getroot(u);
    if (sol[rt] < 0) return sol[rt];
    else return U.sum * sol[rt];
}
}
#undef U
void* __init_sol = []() { return memset(sol, -1, sizeof sol); } (); // 初始全部无解
}
using namespace LCT::Functions;

int main() {
    int n, q;
    scanf("%d", &n);
    for (int i = 1, k, b; i <= n; i++) {
        scanf("%d%d%d", &k, &p[i], &b);
        edit(i, k, b);
    }
    for (int i = 1; i <= n; i++) link(i, p[i]);
    scanf("%d", &q); char ch;
    for (int a, x, y, z, _q = 1; _q <= q; ++_q) {
        scanf( "%c%d", &ch, &a);
        if (ch == 'A') {
            printf("%d\n", query(a));
        } else {
            scanf("%d%d%d", &x, &y, &z);
            edit(a, x, z);
            cut(a, p[a]);
            link(a, p[a] = y);
        }
    }
    return 0;
}