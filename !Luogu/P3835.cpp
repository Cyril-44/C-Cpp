#include <cmath>
#include <stdio.h>
#include <vector>
#include <tuple>
#include <chrono>
#include <random>
constexpr int N = 500005, LG_N = 20, INF = 0x7fffffff;
#ifdef DEBUG
#define debug printf
#else
#define debug(...) ((void)0)
#endif
int rt[N];
std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
namespace FHQ_Treap {
#define U tr[u]
struct Node {
    int val; decltype(rng()) prio; // 这里我建的是小根堆
    int ls, rs;
    int sz, cnt;
    Node(int v = 0, int s = 0) : 
        val(v), prio(rng()), ls(), rs(), sz(s), cnt(s) {}
    Node& L(); Node& R();
    inline void pushup() { sz = L().sz + cnt + R().sz; }
    inline void L(int l) { ls = l; pushup(); }
    inline void R(int r) { rs = r; pushup(); }
    inline void debug_all() { debug("(%10d * %2d_cnt, %2d_sz, l%3dr%3d, %lu_prio)\n", val, cnt, sz, ls, rs, prio); }
};
std::vector<Node> tr;
inline Node& Node::L() { return tr[ls]; }
inline Node& Node::R() { return tr[rs]; }
inline void copy(int &u) {
    tr.emplace_back(U);
    debug("Copied %3d to %3d! ", u, (int)tr.size() - 1);
    u = (int)tr.size() - 1;
}
std::pair<int,int> splitByKey(int &u, int key) {
    if (!u) return {0, 0};
    copy(u);
    if (U.val <= key) {
        auto [ls, rs] = std::move(splitByKey(U.rs, key));
        U.R(ls); return {u, rs};
    }
    auto [ls, rs] = std::move(splitByKey(U.ls, key));
    U.L(rs); return {ls, u};
}
std::tuple<int,int,int> splitByOrd(int &u, int ord) {
    if (!u) return {0, 0, 0};
    copy(u);
    if (U.L().sz < ord && ord <= U.sz - U.R().sz) {
        int tmpl = U.ls, tmpr = U.rs;
        U.L(0); U.R(0); return {tmpl, u, tmpr};
    }
    if (U.L().sz >= ord) {
        auto [ls, targ, rs] = std::move(splitByOrd(U.ls, ord));
        U.L(rs); return {ls, targ, u};
    }
    auto [ls, targ, rs] = std::move(splitByOrd(U.rs, ord - (U.sz - U.R().sz)));
    U.R(ls); return {u, targ, rs};
}
int merge(int &l, int &r) {
    if (!l && !r) return 0;
    if (!l) return r;
    if (!r) return l;
    if (tr[l].prio < tr[r].prio) {
        copy(l), tr[l].R(merge(tr[l].rs, r));
        return l;
    }
    copy(r), tr[r].L(merge(l, tr[r].ls));
    return r;
}
namespace Utils {
inline void insert(int &rt, int val) {
    auto [mid, rs] = splitByKey(rt, val);
    auto [ls, u] = splitByKey(mid, val - 1);
    if (u) ++U.cnt, U.pushup();
    else {
        tr.emplace_back(val, 1);
        u = (int)tr.size() - 1;
    }
    printf("Updated: "); U.debug_all();
    mid = merge(ls, u);
    rt = merge(mid, rs);
}
inline bool erase(int &rt, int val) {
    auto [mid, rs] = splitByKey(rt, val);
    auto [ls, u] = splitByKey(mid, val - 1);
    if (!u) return false;
    --U.cnt;
    if (U.cnt) U.pushup(); else u = 0;
    mid = merge(ls, u);
    rt = merge(mid, rs);
    return true;
}
inline int getOrd(int &rt, int val) {
    auto [l, r] = splitByKey(rt, val - 1);
    int res = tr[l].sz;
    rt = merge(l, r);
    return res;
}
inline int getValByOrd(int &rt, int ord) {
    ++ord; // Guard
    auto [l, u, r] = splitByOrd(rt, ord);
    int res = U.val;
    int mid = merge(l, u);
    rt = merge(mid, r);
    return res;
}
}
inline void debug_all(int u, int dep = 0) {
    if (!u) return;
    debug_all(U.ls, dep + 2);
    printf("#%3d %10d (*%2d): \t", u, U.val, U.cnt);
    for (int i = dep; i--; putchar(' '));
    printf("sz=%d, prio=%lu, ls=%d, rs=%d\n", U.sz, U.prio, U.ls, U.rs);
    debug_all(U.rs, dep + 2);
}
#undef U
}
using namespace FHQ_Treap::Utils;
int main() {
    FHQ_Treap::tr.reserve(N * LG_N * 2);
    insert(rt[0], -INF);
    FHQ_Treap::debug_all(rt[0]); puts("=====================");
    insert(rt[0], INF);
    FHQ_Treap::debug_all(rt[0]); puts("=====================");
    int n;
    scanf("%d", &n);
    for (int i = 1, v, opt, x; i <= n; i++) {
        scanf("%d%d%d", &v, &opt, &x);
        rt[i] = rt[v];
        switch (opt) {
        case 1: insert(rt[i], x); break;
        case 2: erase(rt[i], x); break;
        case 3: printf("%d\n", getOrd(rt[i], x)); break;
        case 4: printf("%d\n", getValByOrd(rt[i], x)); break;
        case 5: printf("%d\n", getValByOrd(rt[i], getOrd(rt[i], x) - 1)); break;
        case 6: printf("%d\n", getValByOrd(rt[i], getOrd(rt[i], x) + 1)); break;
        }
        FHQ_Treap::debug_all(rt[i]);
        debug("============ Finished Query %d ============\n", i);
    }
    return 0;
}
