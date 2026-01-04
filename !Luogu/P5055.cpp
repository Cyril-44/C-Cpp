#include <stdio.h>
#include <algorithm>
#include <vector>
#include <random>
constexpr int N = 200005, LG_N = 19;
int rt[N];
using LL = long long;
std::mt19937 rng(std::random_device{}());
namespace FHQ_Treap {
#define U tr[u]
struct Node {
    int val, sz; decltype(rng()) prio;
    int ls, rs;
    LL sum;
    bool rev;
    Node(int s = 0) : val(s), sz(1), prio(rng()), ls(), rs(), sum(s), rev() {}
    inline void flip() {
        std::swap(ls, rs);
        rev ^= 1;
    }
    Node& L(); Node& R();
    inline void pushup() {
        sz = L().sz + 1 + R().sz;
        sum = L().sum + val + R().sum;
    }
    inline void setL(int x) { ls = x; pushup(); }
    inline void setR(int x) { rs = x; pushup(); }
};
std::vector<Node> tr;
void* _fhq_treap_initialization = [] () {
    tr.reserve(N * LG_N * 4 * 3);
    tr.emplace_back(); tr[0].sz = 0;
    tr.emplace_back(); // Front guard
    tr.emplace_back(); // Back guard
    if (tr[1].prio < tr[2].prio) tr[1].rs = 2, rt[0] = 1;
    else tr[2].ls = 1, rt[0] = 2;
    return nullptr;
} ();
inline Node& Node::L() { return tr[ls]; }
inline Node& Node::R() { return tr[rs]; }
inline void clone(int &u) {
    tr.emplace_back(U);
    u = (int)tr.size() - 1;
}
inline void pushdown(int u) {
    if (!U.rev) return;
    if (U.ls) { clone(U.ls); U.L().flip(); }
    if (U.rs) { clone(U.rs); U.R().flip(); }
    tr[u].rev = false;
}
std::pair<int,int> splitByOrd(int u, int ord) { // [<=ord, > ord] 
    if (!u) return {0, 0};
    pushdown(u);
    clone(u);
    if (U.L().sz + 1 == ord) {
        int rs = U.rs;
        U.setR(0);
        return {u, rs};
    }
    if (U.L().sz + 1 < ord) {
        auto [ls, rs] = splitByOrd(U.rs, ord - U.L().sz - 1);
        U.setR(ls);
        return {u, rs};
    }
    auto [ls, rs] = splitByOrd(U.ls, ord);
    U.setL(rs);
    return {ls, u};
}
int merge(int l, int r) {
    if (!l || !r) return l | r;
    pushdown(l);
    pushdown(r);
    if (tr[l].prio < tr[r].prio) {
        clone(l);
        tr[l].setR(merge(tr[l].rs, r));
        return l;
    }
    clone(r);
    tr[r].setL(merge(l, tr[r].ls));
    return r;
}
namespace Utils {
inline void insertAfter(int &rt, int ord, int ins) {
    ++ord; // Left Guard
    auto [ls, rs] = splitByOrd(rt, ord);
    tr.emplace_back(ins);
    rt = merge(merge(ls, (int)tr.size() - 1), rs);
}
inline void erase(int &rt, int ord) {
    ++ord; // Left Guard
    auto [mid, rs] = splitByOrd(rt, ord);
    auto [ls, u] = splitByOrd(mid, ord - 1);
    rt = merge(ls, rs);
}
inline void flip(int &rt, int l, int r) {
    ++r; // Left Guard, ++l, ++r, l = l-1, r = r
    auto [mid, rs] = splitByOrd(rt, r);
    auto [ls, u] = splitByOrd(mid, l);
    U.flip();
    rt = merge(merge(ls, u), rs);
}
inline LL getsum(int &rt, int l, int r) {
    ++r;
    auto [mid, rs] = splitByOrd(rt, r);
    auto [ls, u] = splitByOrd(mid, l);
    LL res = U.sum;
    rt = merge(merge(ls, u), rs);
    return res;
}
}
void debug_all(int u, int dep = 0) {
    if (!u) return;
    pushdown(u);
    debug_all(U.ls, dep + 2);
    printf("(#%2d)%d:", u, U.val);
    for (int i = dep; i--; putchar(' '));
    printf("sz=%d, sum=%lld, l%dr%d, %lu_prio\n", U.sz, U.sum, U.ls, U.rs, U.prio);
    debug_all(U.rs, dep + 2);
}
#undef U
}
using namespace FHQ_Treap::Utils;
int main() {
    int n;
    scanf("%d", &n);
    // FHQ_Treap::debug_all(rt[0]);
    LL x, y, last_ans = 0;
    for (int i = 1, v, opt; i <= n; i++) {
        scanf("%d%d%lld", &v, &opt, &x);
        if (opt ^ 2) scanf("%lld", &y);
        rt[i] = rt[v];
        x ^= last_ans, y ^= last_ans;
        // printf("%d %d %lld %lld\n", v, opt, x, y);
        switch (opt) {
        case 1: insertAfter(rt[i], x, y); break;
        case 2: erase(rt[i], x); break;
        case 3: flip(rt[i], x, y); break;
        case 4: printf("%lld\n", last_ans = getsum(rt[i], x, y)); break;
        }
        // puts("============================");
        // FHQ_Treap::debug_all(rt[i]);
    }
    return 0;
}