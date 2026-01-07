#include <stdio.h>
#include <algorithm>
#include <string.h>
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
    inline void pushup() { assert(w^1); sum = (*this)(L).sum + (*this)(R).sum; w = (*this)(L).w + (*this)(R).w; }
    inline void pull(LL x) { add += x; sum += x * w; }
    Node() : ch{}, w(), refCnt(), sum(), add() {}
    Node(int v) : ch{}, w(1), refCnt(1), sum(v), add() {}
    Node(int l, int r) : ch{l, r}, w(), refCnt(1), sum(), add() { pushup(); }
} tr[N << 3];
inline Node& Node::operator()(bool c) { return tr[ch[c]]; }
namespace Data {
int bin[N << 3], bintop = 0, top = 0;
namespace Funcs {
template<class... Args> inline int alloc(Args... args) {
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
    if (tr[u].w == 1 || tr[u].refCnt == 1) return;
    --tr[u].refCnt;
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
inline std::pair<int,int> cut(int u) {
    if (tr[u].w == 1) return {0,0};
    pushdown(u);
    ++tr[u](L).refCnt, ++tr[u](R).refCnt;
    recycle(u);
    return {tr[u][L], tr[u][R]};
}
inline bool needBalance(int wl, int wr) { return wr * 3 < wl; }
inline int merge(int l, int r) {
    if (!l || !r) return l | r;
    if (needBalance(tr[l].w, tr[r].w)) { // l too heavy
        auto [ll, lr] = cut(l);
        if (needBalance(tr[lr].w + tr[r].w, tr[ll].w)) {
            auto [lrl, lrr] = cut(lr);
            return alloc(alloc(ll, lrl), alloc(lrr, r));
        }
        return alloc(ll, alloc(lr, r));
    }
    if (needBalance(tr[r].w, tr[l].w)) { // r too heavy
        auto [rl, rr] = cut(r);
        if (needBalance(tr[l].w + tr[rl].w, tr[rr].w)) {
            auto [rll, rlr] = cut(rl);
            return alloc(alloc(l, rll), alloc(rlr, rr));
        }
        return alloc(alloc(l, rl), rr);
    }
    return alloc(l, r);
}
inline std::pair<int,int> split(int u, int ord) {
    assert(0 <= ord && ord <= tr[u].w);
    if (!ord) return {0, u};
    if (ord == tr[u].w) return {u, 0};
    auto [l, r] = cut(u);
    if (ord <= tr[u](L).w) {
        auto [ll, lr] = split(tr[u][L], ord);
        return {ll, merge(lr, r)};
    }
    auto [rl, rr] = split(tr[u][R], ord - tr[u](L).w);
    return {merge(l, rl), rr};
}
int build(int l, int r) {
    if (l == r) return alloc(a[l]);
    int mid = l + r >> 1;
    return alloc(build(l, mid), build(mid + 1, r));
}
int root;
namespace Utils {
void debug_all(int &u = root, int dep = 0) {
    if (!u) return;
    assert(u);
    printf("#%d: ", u);
    if (tr[u].w == 1) printf("(%lld)", tr[u].sum);
    for (int i = dep; i--; putchar(' '));
    if (tr[u].w == 1) printf(" w=%d\n", tr[u].w);
    else {
        printf("%lld w=%d add=%lld\n", tr[u].sum, tr[u].w, tr[u].add); 
        pushdown(u);
        debug_all(tr[u][L], dep + 2);
        debug_all(tr[u][R], dep + 2);
    }
}
inline void init(int n) {
    for (int i = n; i >= 1; i--) a[i+1] = a[i];
    a[0] = a[1] = a[n+2] = a[n+3] = 0;
    root = build(0, n+3); // 前后各两个哨兵
}
#define fetch(L, R) ++L, ++R, ++R; auto [tmp, r] = split(root, R); auto [l, u] = split(tmp, L)
inline void plus(int L, int R, int d) {
    fetch(L, R);
    tr[u].pull(d);
    puts("==== l ===="); debug_all(l);
    puts("==== m ===="); debug_all(u);
    puts("==== r ===="); debug_all(r);
    root = merge(merge(l, u), r);
}
inline void copy(int dL, int dR, int sL, int sR) {
    int src; { // Copy src
        fetch(sL, sR);
        ++tr[src = u].refCnt;
        root = merge(merge(l, u), r);
    }
    fetch(dL, dR); recycle(u);
    root = merge(merge(l, src), r);
}
inline LL querySum(int L, int R) {
    fetch(L, R);
    LL res = tr[u].sum;
    puts("==== l ===="); debug_all(l);
    puts("==== m ===="); debug_all(u);
    puts("==== r ===="); debug_all(r);
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
    debug_all(); fflush(stdout);
    for (int opt, a, b, c, d; m--; ) {
        scanf("%d%d%d", &opt, &a, &b);
        switch (opt) {
        case 1: scanf("%d", &c); plus(a, b, c); break;
        case 2: scanf("%d%d", &c, &d); copy(a, b, c, d); break;
        case 3: printf("%lld\n", querySum(a, b));
        }
        puts("========================"); debug_all();
    }
    return 0;
}