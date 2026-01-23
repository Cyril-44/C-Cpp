#include <algorithm>
#include <array>
#include <set>
#include <stack>
#include <stdio.h>
#include <tuple>
#include <vector>
using LL = long long;
#define U(x) std::get<0>(*x)
#define V(x) std::get<1>(*x)
#define T(x) std::get<2>(*x)
constexpr int N = 100005, M = 200005;

struct { int to, nxt; } e[M << 1];
int head[N];
inline void addedg(int fr, int to) { e[++head[0]] = {to, head[fr]}, head[fr] = head[0]; }

int s[N], uni[N + M], rt[N + M];
int dfn[N], low[N], ts, sta[N], present[N];

int fa[N];
inline int getroot(int u) { return u ^ fa[u] ? fa[u] = getroot(fa[u]) : u; }

void tarjan(int u) {
    dfn[u] = low[u] = ++ts;
    sta[++sta[0]] = u;
    for (int i = head[u], v; i; i = e[i].nxt) {
        if (!dfn[v = e[i].to])
            tarjan(v), low[u] = std::min(low[u], low[v]);
        else
            low[u] = std::min(low[u], dfn[v]);
    }
    if (dfn[u] == low[u]) {
        do present[sta[sta[0]]] = u;
        while (sta[sta[0]--] != u);
    }
}
template <class Iter> void solve(int l, int r, Iter beg, Iter end) { // 时间在 [l, r] 内的进行处理
    if (beg == end) return;
    if (l == r) { // 关键更新：将 beg..end 的边当作无向边且加入时间为 l
        for (Iter i = beg; i != end; ++i) T(i) = l;
        return;
    }
    int mid = l + r >> 1;
    Iter midth = std::upper_bound(beg, end, mid, [](int x, const auto &y) { return x < std::get<2>(y); });
    std::set<int> pnts;
    for (Iter i = beg; i != end; ++i) {
        pnts.insert(U(i));
        pnts.insert(V(i));
    }
    
    head[0] = 0;
    for (int p : pnts) head[p] = dfn[p] = 0, present[p] = -1;
    ts = 0;
    
    for (Iter i = beg; i != midth; ++i) addedg(U(i), V(i));
    for (int p : pnts) if (!dfn[p]) tarjan(p);
    
    // for (int p : pnts) printf("%d(dfn=%d,low=%d) presented as %d\n", p, dfn[p], low[p], present[p]);
    for (Iter i = beg; i != midth;) {
        if (present[U(i)] != present[V(i)]) {
            U(i) = present[U(i)], V(i) = present[V(i)], T(i) = mid + 1;
            std::swap(*i, *(--midth)); // 应当分入 [mid+1, r] 的时间
        } else
            ++i;
    }
    for (Iter i = midth; i != end; ++i) { U(i) = present[U(i)], V(i) = present[V(i)]; }

    solve(l, mid, beg, midth), solve(mid + 1, r, midth, end);
}

namespace SGT {
struct SegNode {
    int ls, rs;
    int cnt;
    LL sum;
};
std::array<SegNode, N * 30> tr;
int bin[N * 20];
inline void pushup(int u) {
    tr[u].sum = tr[tr[u].ls].sum + tr[tr[u].rs].sum;
    tr[u].cnt = tr[tr[u].ls].cnt + tr[tr[u].rs].cnt;
}
inline void mer(int &u, int v, int l, int r) {
    if (!u || !v) u |= v;
    else {
        int mid = l + r >> 1;
        mer(tr[u].ls, tr[v].ls, l, mid);
        mer(tr[u].rs, tr[v].rs, mid + 1, r);
        bin[++bin[0]] = v;
        pushup(u);
    }
}
int totcnt = 0;
int P, X, Y;
inline void upd(int &u, int l, int r) {
    if (!u) tr[u = (bin[0] ? bin[bin[0]--] : ++totcnt)] = {0, 0, 0};
    if (l == r) tr[u].sum += X, tr[u].cnt += Y;
    else {
        int mid = l + r >> 1;
        if (P <= mid) upd(tr[u].ls, l, mid);
        else upd(tr[u].rs, mid + 1, r);
        pushup(u);
    }
}
inline LL que(int u, int l, int r, int rnk) {
    if (!u || !rnk) return 0;
    if (l == r || rnk == tr[u].cnt) return tr[u].sum;
    int mid = l + r >> 1;
    if (tr[tr[u].rs].sum >= X) return que(tr[u].rs, mid + 1, r, rnk);
    return tr[tr[u].rs].sum + que(tr[u].ls, l, mid, rnk - tr[tr[u].rs].cnt);
}
inline LL query(int x, int r) {
    return que(rt[getroot(x)], 1, uni[0], r);
}
inline void update(int u, int p, int x, int y) {
    P = p, X = x, Y = y;
    upd(rt[getroot(u)], 1, uni[0]);
}
inline void merge(int x, int y) {
    mer(rt[x], rt[y], 1, uni[0]);
}
inline void debug(int x) {
    if (!x) return;
    if (!tr[x].ls && !tr[x].rs) {
        printf("(%lld,%d) ", tr[x].sum, tr[x].cnt);
    } else {
        debug(tr[x].ls);
        debug(tr[x].rs);
    }
}
} // namespace SGT

inline void unite(int u, int v) {
    u = getroot(u), v = getroot(v);
    if (u ^ v) {
        // printf("Unite %d to %d ", u, v);
        fa[u] = v, SGT::merge(v, u);
        // SGT::debug(v); putchar('\n');
    }
}
inline int getrnk(int s) {
    return std::lower_bound(uni + 1, uni + 1 + uni[0], s) - uni;
}

int main() {
    int n, m, q;
    scanf("%d%d%d", &n, &m, &q);
    std::vector<std::pair<int, int>> edgs;
    std::vector<std::tuple<int, int, int>> adds, ques;
    std::set<std::pair<int, int>> deleted;
    std::vector<LL> ans;
    ans.reserve(q), edgs.reserve(m), ques.reserve(q + 1), adds.reserve(m);
    for (int i = 1; i <= n; i++) fa[i] = i;

    for (int i = 1; i <= n; i++) scanf("%d", &s[i]), uni[++uni[0]] = s[i];
    for (int i = 1, x, y; i <= m; i++) {
        scanf("%d%d", &x, &y);
        edgs.emplace_back(x, y);
    }
    ques.emplace_back();
    for (int i = q, opt, x, y; i >= 1; i--) {
        scanf("%d%d%d", &opt, &x, &y);
        if (opt == 1)
            adds.emplace_back(x, y, i), deleted.emplace(x, y);
        else if (opt == 2)
            s[x] += y, uni[++uni[0]] = s[x];
        ques.emplace_back(opt, x, y);
    }
    // for (auto i = adds.rbegin(); i != adds.rend(); ++i) { printf("%d->%d[%d], ", U(i), V(i), T(i)); } putchar('\n');
    std::sort(uni + 1, uni + 1 + uni[0]);
    uni[0] = std::unique(uni + 1, uni + 1 + uni[0]) - uni - 1;
    for (const auto &i : edgs)
        if (!deleted.count(i)) adds.emplace_back(i.first, i.second, 0);

    // for (auto it = adds.rbegin(); it != adds.rend(); ++it) { printf("%d->%d when time %d\n", U(it), V(it), T(it)); }
    solve(0, q+1, adds.rbegin(), adds.rend());
    // for (auto it = adds.rbegin(); it != adds.rend(); ++it) { printf("%d->%d when time %d\n", U(it), V(it), T(it)); }

    for (int i = 1; i <= n; i++) { SGT::update(i, getrnk(s[i]), s[i], 1); }
    std::reverse(ques.begin()+1, ques.end());
    auto it = adds.rbegin();
    for (int i = 0; i <= q; i++) {
        for (; it != adds.rend() && T(it) <= i; ++it) unite(U(it), V(it));
        const auto &[opt, x, y] = ques[i];
        switch (opt) {
        case 2:
            // printf("Make #%d: %d --> %d\n", x, s[x], s[x] - y);
            SGT::update(x, getrnk(s[x]), -s[x], -1);
            s[x] -= y;
            SGT::update(x, getrnk(s[x]), s[x], 1);
            break;
        case 3:
            ans.push_back(SGT::query(x, y));
        }
        /* printf("After oper %d: \n", i);
        for (int i = 1; i <= n; i++) {
            if (fa[i] == i) {
                printf("Group %d: ", i);
                SGT::debug(rt[i]);
                putchar('\n');
            }
        } */
    }
    for (auto it = ans.rbegin(); it != ans.rend(); ++it) printf("%lld\n", *it);
    return 0;
}