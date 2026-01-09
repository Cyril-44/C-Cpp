#include <stdio.h>
#include <algorithm>
#include <assert.h>
constexpr int N = 100005;
using LL = long long;
int a[N];
namespace WBLT {
enum {L, R};
struct Node {
    int ch[2], w, refCnt;
    LL sum, add; // tag
    Node& operator()(bool c);
    inline int& operator[](bool c) { return ch[c]; }
    inline void pushup() { if (w^1) sum = (*this)(L).sum + (*this)(R).sum, w = (*this)(L).w + (*this)(R).w; }
    inline void pull(LL x) { add += x; sum += x * w; }
    Node() : ch{}, w(), refCnt(), sum(), add() {}
    Node(int v) : ch{}, w(1), refCnt(1), sum(v), add() {}
    Node(int l, int r) : ch{l, r}, w(), refCnt(1), sum(), add() { pushup(); }
} tr[N << 4];
inline Node& Node::operator()(bool c) { return tr[ch[c]]; }

namespace Data {
int bin[N << 4], bintop = 0, top = 0;
namespace Funcs {
template<class... Args> inline int alloc(Args... args) {
    // if (top >= (N << 4)) exit(0);
    int u = bintop ? bin[bintop--] : ++top;
    assert(u);
    tr[u] = Node(args...); return u;
}
inline void recycle(int u) {
    assert(u);
    if (u && --tr[u].refCnt == 0) {
        if (tr[u].w ^ 1) recycle(tr[u][L]), recycle(tr[u][R]);
        bin[++bintop] = u;
    }
}
inline void check(int &u) {
    if (tr[u].refCnt == 1) return;
    --tr[u].refCnt;
    if (tr[u].w != 1) ++tr[u](L).refCnt, ++tr[u](R).refCnt;
    tr[u = alloc(tr[u])].refCnt = 1;
}
} // namespace Funcs
} // namespace Data
using namespace Data::Funcs;

inline void pushdown(int &u) {
    if (tr[u].w == 1 || !tr[u].add) return;
    check(u);
    if (tr[u][L]) check(tr[u][L]), tr[u](L).pull(tr[u].add);
    if (tr[u][R]) check(tr[u][R]), tr[u](R).pull(tr[u].add);
    tr[u].add = 0;
}
inline std::pair<int,int> cut(int& u) {
    assert(u);
    if (tr[u].w == 1) return {0,0};
    pushdown(u);
    ++tr[u](L).refCnt, ++tr[u](R).refCnt;
    recycle(u);
    std::pair<int,int> ret(tr[u][L], tr[u][R]);
    u = 0;
    return ret;
}

[[gnu::always_inline]] inline bool needBalance(int wl, int wr) { return wr * 3 < wl; }
[[gnu::always_inline]] inline bool needDoubleRot(int u, bool x) { return tr[u](x).w * 2 < tr[u](!x).w; }
[[gnu::always_inline]] inline void rotate(int &u, bool x) { // 将 u->x 旋转到 u
    auto [l, r] = cut(u);
    if (x) { // r heavier
        auto [rl, rr] = cut(r);
        u = alloc(alloc(l, rl), rr);
    } else {
        auto [ll, lr] = cut(l);
        u = alloc(ll, alloc(lr, r));
    }
}
/* An Example: 
        u              u
       / \            / \
      3   1          /   \
     / \     ===>   2     2
    1   2          / \   / \
       / \        1   1 1   1
      1   1                           */
[[gnu::always_inline]] inline void balance(int &u) {
    if (tr[u].w == 1) return;
    bool x = tr[u](R).w > tr[u](L).w; // 重儿子编号
    if (!needBalance(tr[u](x).w, tr[u](!x).w)) return;
    if (needDoubleRot(tr[u][x], x)) rotate(tr[u][x], !x); // 先一边倒，然后拉回来
    rotate(u, x);
}
inline int merge(int l, int r) {
    if (!l || !r) return l | r;
    if (needBalance(tr[l].w, tr[r].w)) { // l too heavy
        auto [ll, lr] = cut(l);
        int u = alloc(ll, alloc(lr, r));
        balance(u); return u;
    }
    if (needBalance(tr[r].w, tr[l].w)) { // r too heavy
        auto [rl, rr] = cut(r);
        int u = alloc(alloc(l, rl), rr);
        balance(u); return u;
    }
    return alloc(l, r);
}
inline std::pair<int,int> split(int u, int ord) {
    assert(0 <= ord && ord <= tr[u].w);
    if (!ord) return {0, u};
    if (ord == tr[u].w) return {u, 0};
    auto [l, r] = cut(u);
    if (ord <= tr[l].w) {
        auto [ll, lr] = split(l, ord);
        return {ll, merge(lr, r)};
    }
    auto [rl, rr] = split(r, ord - tr[l].w);
    return {merge(l, rl), rr};
}
int build(int l, int r) {
    if (l == r) return alloc(a[l]);
    int mid = l + r >> 1;
    return alloc(build(l, mid), build(mid + 1, r));
}
int root;

namespace Utils {
inline void init(int n) {
    for (int i = n; i >= 1; i--) a[i+1] = a[i];
    a[0] = a[1] = a[n+2] = a[n+3] = 0;
    root = build(0, n+3); // 前后各两个哨兵
}
#define fetch(L, R) ++L, ++R, ++R; auto [tmp, r] = split(root, R); auto [l, u] = split(tmp, L)
inline void plus(int L, int R, int d) {
    fetch(L, R);
    tr[u].pull(d);
    root = merge(merge(l, u), r);
}
inline void copy(int dL, int dR, int sL, int sR) {
    int src; { // Copy src
        fetch(sL, sR);
        ++tr[src = u].refCnt;
        root = merge(merge(l, u), r);
    }
    fetch(dL, dR);
    root = merge(merge(l, src), r);
    recycle(u);
}
inline LL querySum(int L, int R) {
    fetch(L, R);
    LL res = tr[u].sum;
    root = merge(merge(l, u), r);
    return res;
}
#undef fetch
} // namespace Utils
} // namespace WBLT
using namespace WBLT::Utils;
int main() {
    int n, m;
    scanf("%*d%d%d", &n, &m);
    for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
    init(n);
    for (int opt, a, b, c, d; m--; ) {
        scanf("%d%d%d", &opt, &a, &b);
        switch (opt) {
        case 1: scanf("%d", &c); plus(a, b, c); break;
        case 2: scanf("%d%d", &c, &d); copy(a, b, c, d); break;
        case 3: printf("%lld\n", querySum(a, b)); break;
        }
    }
    return 0;
}