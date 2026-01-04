#include <stdio.h>
#include <algorithm>
#include <vector>
#include <random>
constexpr int N = 300005, MOD = (int)1e9 + 7;
int rt[N];
std::mt19937 rng(std::random_device{}());
namespace FHQ_Treap {
#define U tr[u]
inline void Add(int &x, int y) { if ((x += y) >= MOD) x -= MOD; }
struct Node {
    decltype(rng()) prio;
    int val, sum, sz;
    int ls, rs;
    int cov, add;
    int refer;
    bool rev;
    Node(int x = 0) : prio(rng()), val(x), sum(x), sz(1), ls(), rs(), cov(-1), add() {}
    Node& L(); Node& R();
    inline void pushup() {
        sz = L().sz + 1 + R().sz;
        Add(sum = val, L().sum);
        Add(sum, R().sum);
    }
    inline void cover(int x) {
        if (x == -1) return;
        add = 0, val = cov = x;
        sum = 1ull * x * sz % MOD;
    }
    inline void plus(int x) {
        if (!x) return;
        Add(add, x), Add(val, x);
        Add(sum, 1ull * x * sz % MOD);
    }
    void referTo(int);
    inline void reverse() {
        std::swap(ls, rs);
        rev ^= 1;
    }
    inline void setL(int x) { ls = x; pushup(); }
    inline void setR(int x) { rs = x; pushup(); }
};
std::vector<Node> tr;
inline Node& Node::L() { return refer ? tr[tr[refer].ls] : tr[ls]; }
inline Node& Node::R() { return refer ? tr[tr[refer].rs] : tr[rs]; }
inline void Node::referTo(int u) {
    refer = u;
    sum = U.sum;
    add = 0, cov = -1, rev = -1;
}
int rt;
void* _fhq_treap_initialization = []() {
    tr.reserve(N);
    tr.emplace_back(); tr[0].sz = 0;
    tr.emplace_back(); tr.emplace_back();
    if (tr[1].prio < tr[2].prio) tr[rt = 1].rs = 2;
    else tr[rt = 2].ls = 1;
    return nullptr;
}();
inline void pushdown(int u) {
    if (U.ls) U.L().cover(U.cov), U.L().plus(U.add);
    if (U.rs) U.R().cover(U.cov), U.R().plus(U.add);
    U.cov = -1, U.add = 0;
}
std::pair<int,int> splitByOrd(int u, int ord) {
    if (!u) return {0,0};
    if (U.L().sz + 1 == ord) {
        int rs = U.rs;
        U.setR(0);
        return {u, rs};
    }
    if (U.L().sz < ord) {
        auto [ls, rs] = splitByOrd(U.rs, ord);
        U.setR(ls);
        return {u, rs};
    }
    auto [ls, rs] = splitByOrd(U.ls, ord);
    U.setL(rs);
    return {ls, u};
}
int merge(int l, int r) {
    if (!l || !r) return l | r;
    if (tr[l].prio < tr[r].prio) return tr[l].setR(merge(tr[l].rs, r)), l;
    return tr[r].setL(merge(l, tr[r].ls)), r;
}
namespace Utils {
inline void insert(int x) {
    auto [ls, rs] = splitByOrd(rt, tr[rt].sz - 1);
    tr.emplace_back(x);
    rt = merge(merge(ls, (int)tr.size() - 1), rs);
}
inline void copyto(int destL, int destR, int srcL, int srcR) {
    
}
}
#undef U
}
int main() {
    int n, m;
    scanf("%d%d", &n, &m);

}
