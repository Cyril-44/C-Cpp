#include <array>
#include <cfloat>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <random>
constexpr int N = 300005, MOD = (int)1e9 + 7;
int rt[N];
enum ProtoType1 { ADD, COV, FLP };
enum ProtoType2 { COPY, SWAP };
std::mt19937 rng(std::random_device{}());
namespace FHQ_Treap {
#define U tr[u]
inline void Add(int &x, int y) { if ((x += y) >= MOD) x -= MOD; }
struct Node {
    friend inline void addRef(Node* u) { u && ++u->refCnt; }
    friend inline void delRef(Node* u) { if (u && --u->refCnt == 0) delete u; }
    friend inline void clone(Node*& u) {
        if (u->refCnt == 1) return;
        auto ptr = new Node(*u);
        --u->refCnt; ptr->refCnt = 1;
        addRef(u->ls), addRef(u->rs);
        u = ptr;
    }
    Node *ls, *rs;
    decltype(rng()) prio;
    int val, sum, sz;
    int cov, add;
    int refCnt;
    bool rev;
    inline Node(int x = 0) : ls(), rs(), prio(rng()), val(x), sum(x), sz(1), cov(-1), add(), refCnt(1), rev() {}
    inline ~Node() { delRef(ls); delRef(rs); }
    inline void pushup() {
        sz = (ls ? ls->sz : 0) + 1 + (rs ? rs->sz : 0);
        sum = val;
        if (ls) Add(sum, ls->sum);
        if (rs) Add(sum, rs->sum);
    }
    inline void pushdown() {
        if (ls) { clone(ls); if (rev) ls->flip(); ls->cover(cov), ls->plus(add); }
        if (rs) { clone(rs); if (rev) rs->flip(); rs->cover(cov), rs->plus(add); }
        cov = -1, add = 0;
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
    inline void flip() {
        std::swap(ls, rs);
        rev ^= 1;
    }
    inline void setL(Node* x) { ls = x; pushup(); }
    inline void setR(Node* x) { rs = x; pushup(); }
};
using Fn_t1 = void(*) (Node*, int);
using Fn_t2 = void(*) (Node*&, Node*&);
Node* rt;
void debug_all(Node *u = rt, int dep = 0) {
    if (!u) return;
    u->pushdown();
    debug_all(u->ls, dep + 2);
    printf("%2d: ", u->val);
    for (int i = dep; i--; putchar(' '));
    printf("sum=%d, sz=%d, prio_%lu\n", u->sum, u->sz, u->prio);
    debug_all(u->rs, dep + 2);
}
void* _fhq_treap_initialization = []() {
    auto l = new Node(), r = new Node();
    if (l->prio < r->prio) (rt = l)->rs = r;
    else (rt = r)->ls = l;
    rt->pushup();
    l->refCnt = r->refCnt = 1919810; // Prevent from being deleted
    return nullptr;
}();
std::pair<Node*,Node*> splitByOrd(Node *u, int ord) {
    if (!u) return {0,0};
    clone(u);
    u->pushdown();
    int lsz = u->ls ? u->ls->sz : 0;
    if (lsz + 1 == ord) {
        auto rs = u->rs;
        u->setR(nullptr);
        return {u, rs};
    }
    if (lsz < ord) {
        auto [ls, rs] = splitByOrd(u->rs, ord - lsz - 1);
        u->setR(ls);
        return {u, rs};
    }
    auto [ls, rs] = splitByOrd(u->ls, ord);
    u->setL(rs);
    return {ls, u};
}
Node* merge(Node* l, Node* r) {
    if (!l) return r;
    if (!r) return l;
    clone(l); clone(r);
    l->pushdown();
    r->pushdown();
    if (l->prio < r->prio) return l->setR(merge(l->rs, r)), l;
    return r->setL(merge(l, r->ls)), r;
}
Fn_t1 oneRangeFuncs[3]{
    [](Node *u, int x){ u->plus(x); },
    [](Node *u, int x){ u->cover(x); },
    [](Node *u, int){ u->flip(); }
};
Fn_t2 twoRangeFuncs[2]{
    [](Node *&dest, Node *&src){ clone(dest = src); },
    [](Node *&dest, Node *&src){ std::swap(dest, src); }
};
namespace Utils {
inline void push_back(int x) {
    auto [ls, rs] = splitByOrd(rt, rt->sz - 1);
    rt = merge(merge(ls, new Node(x)), rs);
}
inline void modify(int destL, int destR, int srcL, int srcR, ProtoType2 tp) {
    ++destR, ++srcR;
    std::array<std::pair<int,int>, 2> sorted{std::pair<int,int>{destL, destR}, std::pair<int,int>{srcL, srcR}};
    std::sort(sorted.begin(), sorted.end());
    auto [tmp1, rs] = splitByOrd(rt, sorted[1].second);
    auto [tmp2, u2] = splitByOrd(tmp1, sorted[1].first);
    auto [tmp3, mid] = splitByOrd(tmp2, sorted[0].second);
    auto [ls, u1] = splitByOrd(tmp3, sorted[0].first);
    /* printf("==== ls ====\n"), debug_all(ls);
    printf("==== u1 ====\n"), debug_all(u1);
    printf("==== md ====\n"), debug_all(mid);
    printf("==== u2 ====\n"), debug_all(u2);
    printf("==== rs ====\n"), debug_all(rs);
    printf("==== [%d, %d] ==== [%d, %d] ====\n", sorted[0].first, sorted[0].second, sorted[1].first, sorted[1].second);
    fflush(stdout); */
    if (std::make_pair(destL, destR) != sorted.front()) twoRangeFuncs[tp](u2, u1);
    else twoRangeFuncs[tp](u1, u2);
    rt = merge(merge(merge(ls, u1), mid), merge(u2, rs));
}
inline void modify(int l, int r, ProtoType1 tp, int x = 0) {
    auto [mid, rs] = splitByOrd(rt, ++r);
    auto [ls, u] = splitByOrd(mid, l);
    /* printf("====== ls ======\n"), debug_all(ls);
    printf("====== u =======\n"), debug_all(u);
    printf("====== rs ======\n"), debug_all(rs); */
    oneRangeFuncs[tp](u, x);
    rt = merge(merge(ls, u), rs);
}
inline int querySum(int l, int r) {
    auto [mid, rs] = splitByOrd(rt, ++r);
    auto [ls, u] = splitByOrd(mid, l);
    auto res = u->sum;
    rt = merge(merge(ls, u), rs);
    return res;
}
void extract(std::vector<int>& arr, Node *u = rt) {
    if (!u) return;
    u->pushdown();
    extract(arr, u->ls);
    arr.push_back(u->val);
    extract(arr, u->rs);
}
}
#undef U
}
using namespace FHQ_Treap::Utils;
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int i = 1, x; i <= n; i++) {
        scanf("%d", &x);
        push_back(x);
    }
    FHQ_Treap::debug_all();
    for (int opt, a, b, c, d, last = 0, i = 1; i <= m; i++) {
        scanf("%d%d%d", &opt, &a, &b);
        if (opt >= 2 && opt <= 5) scanf("%d", &c);
        if (opt == 4 || opt == 5) scanf("%d", &d);
        a ^= last, b ^= last, c ^= last, d ^= last;
        a %= MOD, b %= MOD, c %= MOD, d %= MOD;
        switch (opt) {
        case 1: printf("%d\n", last = querySum(a, b)); break;
        case 2: modify(a, b, COV, c); break;
        case 3: modify(a, b, ADD, c); break;
        case 6: modify(a, b, FLP); break;
        case 4: modify(c, d, a, b, COPY); break;
        case 5: modify(c, d, a, b, SWAP); break;
        }
        puts("==========================="); FHQ_Treap::debug_all(); fflush(stdout);
    }
    std::vector<int> ans;
    ans.reserve(n + 2);
    extract(ans);
    for (int i = 1; i <= n; i++)
        printf("%d ", ans[i]);
    putchar('\n');
    delete FHQ_Treap::rt;
}
