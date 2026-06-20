#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
#include <cassert>
constexpr int N = 200005;
int n, d[N], lim[N];
int64_t dsum[N];
struct Que {
    int l, r, x;
} ques[N];
struct Node {
    struct Op {
        int64_t add, lmt; // (a1,l1) + (a2,l2) ==> (a1+a2, Min(l2, l1+a2))
        Op& operator+=(const Op &o) { lmt = std::min(lmt + add, o.lmt), add += o.add; return *this; }
        friend Op operator+(Op x, const Op &y) { return x += y; }
        friend int64_t& operator+=(int64_t &x, const Op &y) { return x = std::min(x + y.add, y.lmt); }
        friend int64_t operator+(int64_t x, const Op &y) { return x += y; }
    };
    std::vector<Op> pre, suf, cur;
    Op sum;
} tr[N << 2];
bool cmp(const Node::Op &x, const Node::Op &y) { return x.add < y.add; }
bool cmpl(const Node::Op &x, const Node::Op &y) { return x.lmt < y.lmt; }
bool cmpr(const Node::Op &x, const Node::Op &y) { return x.lmt - x.add < y.lmt - y.add; }
#define ls (u<<1)
#define rs (u<<1|1)
inline void unique(std::vector<Node::Op>& vec) {
    std::sort(vec.rbegin(), vec.rend(), cmp);
    int64_t semx = -1;
    int tot = 0;
    for (int i = 0; i < (int)vec.size(); i++) {
        if (vec[i].lmt <= semx) continue;
        semx = vec[i].lmt;
        vec[tot++] = vec[i];
    }
    vec.resize(tot);
}
void build(int u = 1, int l = 1, int r = n) {
    if (l == r) {
        tr[u].pre = tr[u].suf = tr[u].cur = {(tr[u].sum = {d[l], lim[l]})};
        return;
    }
    tr[u].sum = tr[ls].sum + tr[rs].sum;
    auto lv = tr[ls].suf, rv = tr[rs].pre;
    std::sort(lv.rbegin(), lv.rend(), cmpl);
    std::sort(rv.rbegin(), rv.rend(), cmpr);
    tr[u].cur.reserve(lv.size() + rv.size());
{   // When: Min(l2, l1+a2) = l2
    Node::Op mx{-1, 0};
    int il = 0;
    for (int ir = 0; ir < (int)rv.size(); ir++) {
        for (; il < (int)lv.size() && lv[il].lmt >= rv[ir].lmt - rv[ir].add; il++)
            if (lv[il].add > mx.add) mx = lv[il];
        if (~mx.add) tr[u].cur.emplace_back(mx + rv[ir]);
    }
} { // When: Max(l2, l1+a2) = l1+a2
    Node::Op mx{-1, 0};
    int ir = 0;
    for (int il = 0; il < (int)lv.size(); il++) {
        for (; ir < (int)rv.size() && rv[ir].lmt - rv[ir].add >= lv[il].lmt; ir++)
            if (rv[ir].add > mx.add) mx = rv[ir];
        if (~mx.add) tr[u].cur.emplace_back(mx + rv[ir]);
    }
} { // Calc pre := L.pre + (L.sum + R.pre)
    tr[u].pre = tr[ls].pre;
    tr[u].pre.reserve(tr[ls].pre.size() + tr[rs].pre.size());
    for (const auto &rvec : rv) tr[u].pre.emplace_back(tr[ls].sum + rvec);
} { // Calc suf := (L.suf + R.sum) + R.suf
    tr[u].suf = tr[rs].suf;
    tr[u].suf.reserve(tr[ls].suf.size() + tr[rs].suf.size());
    for (const auto &lvec : lv) tr[u].suf.emplace_back(lvec + tr[rs].sum);
}   unique(tr[u].pre), unique(tr[u].suf), unique(tr[u].cur);
}
inline int64_t calcMx(const std::vector<Node::Op> &vec, int64_t x) { // 在一堆 Op 里面找到最优的值，这里的 Op 是 add 递减 lmt 递增
    int l = 0, r = vec.size() - 1;
    while (l <= r) {
        int mid = l + r >> 1;
        if (x + vec[mid].add <= vec[mid].lmt) r = mid - 1;
        else l = mid + 1;
    }
    int64_t ans = -1;
    if (r >= 0) ans = std::max(ans, x + vec[r]);
    if (l < (int)vec.size()) ans = std::max(ans, x + vec[l]);
    // assert(~ans);
    return ans;
}
class Inquire {
    int64_t ans, preSufmx; // 已经处理的前面范围中的最大后缀
    int x, L, R;
    void inq(int u, int l, int r) { // 线段树查询
        if (L <= l && r <= R) {
            ans = std::max(ans, calcMx(tr[u].pre, preSufmx));
            preSufmx = std::max(preSufmx + tr[u].sum, calcMx(tr[u].suf, x));
        } else {
            int mid = l + r >> 1;
            inq(u<<1, l, mid);
            inq(u<<1|1, mid+1, r);
        }
    }
public:
    inline int64_t operator()(int x, int l, int r) {
        ans = preSufmx = x = x, L = l, R = r;
        inq(1, 1, n);
    }
};
int main() {
    freopen("park1.in", "r", stdin);
    std::cin.tie(nullptr)->sync_with_stdio(false);
    int q; std::cin >> n >> q;
    for (int i = 1; i <= n; i++) std::cin >> d[i], dsum[i] = dsum[i-1] + d[i];
    for (int i = 1; i <= n; i++) std::cin >> lim[i];
    build();
    for (int l, r, x; q--; ) {
        scanf("%d%d%d", &l, &r, &x);
        printf("%lld\n", Inquire{}(x, l, r));
    }
    return 0;
}