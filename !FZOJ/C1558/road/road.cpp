#include <stdio.h>
#include <algorithm>
#include <set>
constexpr int N = 200005;
int n;
namespace VSGT {
struct Node { int mn, ls, rs; } tr[N * 50]{(int)2e9 + 5};
int P, X, tot, rt;
void upd(int& u, int l, int r) {
    if (!u) u = ++tot, (l == r) && (tr[u].mn = 2 * (n-1) - (n-l));
    if (l == r) return tr[u].mn += X, void();
    int mid = l + r >> 1;
    (P <= mid) ? upd(tr[u].ls, l, mid) : upd(tr[u].rs, mid+1, r);
    tr[u].mn = std::min(tr[tr[u].ls].mn, tr[tr[u].rs].mn);
}
inline void update(int p, int x) { P = p, X = x; upd(rt, 1, n); }
inline int getmn() { return tr[rt].mn; }
} using VSGT::update; using VSGT::getmn;
int main() {
    int m;
    scanf("%d%d", &n, &m);
    VSGT::update(1, 0); // Make point 1 exist
    std::set<std::pair<int,int>> st;
    for (int u, v; m--;) {
        scanf("%d%d", &u, &v);
        if (u > v) std::swap(u, v);
        if (st.count({u, v})) { st.erase({u, v}); update(u, -1); update(v, 1); }
        else { st.emplace(u, v); update(u, 1); update(v, -1); }
        printf("%d\n", getmn());
    }
    return 0;
}

/*
考虑最优走法一定能走到所有的点，因为如果我们在考虑 u 的情况下如果走不到 v，那就将起点换成 v，因为一定存在 v->u 的边。
    同理如果 v 走不到 w，那就存在 w->v 的边，而他们彼此是不相同的（否则就矛盾了）。
考虑最优解法的起点到其他的点至多经过两个点。可以进行反证法，同上假设当前选的 u --> v --> w --> x，到 x 至少 3 步，那就以 x 为起点再求。直到整张图都优化完为止（不存在这种情况）。
考虑如果直接的不行，那么就+1的惩罚，反之-1。
输出全部惩罚中最小的。
部分惩罚可能过小，但是无关紧要，可以证明他们的惩罚一定>=指向他们的点。
*/