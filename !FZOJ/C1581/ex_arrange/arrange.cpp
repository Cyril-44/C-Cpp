#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <tuple>
using namespace std;
constexpr int N = 300005;
static int a[N], b[N];
using pii_t = std::pair<int,int>;
#define ls (tr[u].data.v.l)
#define rs (tr[u].data.v.r)
#define tag(u) (tr[u].data.v.add)
#define dv(u) (tr[u].data.val)
#define dpr(...) fprintf(stderr, __VA_ARGS__)
struct Node;
static void pushup(Node&, int, int);
static struct Node {
    union {
        struct {int l, r, add;} v;
        int val;
    } data;
    int sz, mn;
    Node(int x=0) : data{.val=x}, sz(1), mn(x) {}
    Node(int l, int r) : data{.v={l,r,0}} { pushup(*this, l, r); }
} tr[N << 2];
static int bin[N << 2], tot;
template<class... Args> inline static int alloc(Args... args) {
    int u = bin[0] ? bin[bin[0]--] : ++tot;
    tr[u] = Node(args...);
    return u;
}
inline static void apply(int u, int x) {
    tr[u].mn += x;
    if (tr[u].sz == 1) dv(u) += x;
    else tag(u) += x;
}
inline static void recycle(int id) { bin[++bin[0]] = id; }
inline static void pushdown(int u) {
    if (tag(u)) {
        apply(ls, tag(u));
        apply(rs, tag(u));
        tag(u) = 0;
    }
}
inline static void pushup(Node &x, int l, int r) {
    x.sz = tr[l].sz + tr[r].sz;
    x.mn = std::min(tr[l].mn, tr[r].mn);
}
inline static pii_t cut(int u) {
    pushdown(u);
    recycle(u);
    return {ls, rs};
}
inline static bool needBalance(int usz, int vsz) {
    return vsz * 3 < usz;
}
static int build(int l, int r, int *a) {
    if (l == r) return alloc(a[l]);
    int mid = l + r >> 1;
    return alloc(build(l, mid, a), build(mid+1, r, a));
}
static int merge(int u, int v) {
    if (!u || !v) return u | v;
    if (needBalance(tr[u].sz, tr[v].sz)) {
        auto [ul, ur] = cut(u);
        if (needBalance(tr[ur].sz + tr[v].sz, tr[ul].sz)) {
            auto [url, urr] = cut(ur);
            return alloc(merge(ul, url), merge(urr, v));
        }
        return alloc(ul, merge(ur, v));
    }
    if (needBalance(tr[v].sz, tr[u].sz)) {
        auto [vl, vr] = cut(v);
        if (needBalance(tr[u].sz + tr[vl].sz, tr[vr].sz)) {
            auto [vll, vlr] = cut(vl);
            return alloc(merge(u, vll), merge(vlr, vr));
        }
        return alloc(merge(u, vl), vr);
    }
    return alloc(u, v);
}
static pii_t split(int u, int w) { // 左边包含第 w 个
    if (w == 0) return {0, u};
    if (tr[u].sz <= w) return {u, 0};
    auto [ul, ur] = cut(u);
    if (tr[ul].sz >= w) {
        auto [ull, ulr] = split(ul, w);
        return {ull, merge(ulr, ur)};
    }
    auto [url, urr] = split(ur, w - tr[ul].sz);
    return {merge(ul, url), urr};
}
static int at(int u, int w) { // 查询 tr[u] 中的第 w 大
    if (tr[u].sz == 1) {
        return dv(u);
    }
    pushdown(u);
    if (tr[ls].sz >= w) return at(ls, w);
    return at(rs, w - tr[ls].sz);
}
inline static int inqmin(int u, int l, int r) {
    if (l == 1 && r == tr[u].sz) return tr[u].mn;
    pushdown(u); int mid = tr[ls].sz;
    if (mid >= r) return inqmin(ls, l, r);
    if (mid < l) return inqmin(rs, l-mid, r-mid);
    return std::min(inqmin(ls, l, mid), inqmin(rs, 1, r-mid));
}
inline static std::tuple<int,int,int> split(int u, int l, int r) {
    auto [ul, tmp] = std::move(split(u, l-1));
    auto [target, ur] = std::move(split(tmp, r-l+1));
    return {ul, target, ur};
}
inline static void swap(int &u, int x, int y) {
    if (x > y) std::swap(x, y);
    auto [l, ux, tmp] = split(u, x, x);
    auto [mid, uy, r] = split(tmp, y-x, y-x);
    u = merge(merge(merge(l, uy), mid), merge(ux, r));
}
static void print(int u) {
    if (tr[u].sz == 1) std::cout << dv(u) << ' ';
    else pushdown(u), print(ls), print(rs);
}
int main() {
    int n, q;
    std::cin >> n >> q;
    for (int i = 1; i <= n; i++)
        std::cin >> a[i], b[a[i]] = i;
    int tra = build(1, n, a), trb = build(1, n, b);
    for (int op, x, y; q--; ) {
        std::cin >> op >> x;
        if (op > 1) std::cin >> y;
        switch (op) {
        case 1:
            std::cout << at(tra, x) << '\n';
        break;
        case 2: {
            int amin = inqmin(tra, x, y);
            int bx = amin, by = bx + (y-x);
            int bmin = inqmin(trb, bx, by);
            auto [al, au, ar] = split(tra, x, y);
            auto [bl, bu, br] = split(trb, bx, by);
            apply(au, -amin + bmin);
            apply(bu, -bmin + amin);
            tra = merge(merge(al, bu), ar);
            trb = merge(merge(bl, au), br);
        } break;
        case 3: if (x != y) {
            int ax = at(tra, x), ay = at(tra, y);
            swap(tra, x, y), swap(trb, ax, ay);
        } break;
        }
    }
    print(tra);
    return 0;
}