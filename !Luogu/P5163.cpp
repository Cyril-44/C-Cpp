#include <cassert>
#include <cstdio>
#include <algorithm>
#include <array>
#include <set>
#include <stack>
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
bool insta[N];

int fa[N];
inline int getroot(int u) { return u ^ fa[u] ? fa[u] = getroot(fa[u]) : u; }

void tarjan(int u) {
    dfn[u] = low[u] = ++ts;
    sta[++sta[0]] = u, insta[u] = true;
    for (int i = head[u], v; i; i = e[i].nxt) {
        if (!dfn[v = e[i].to])
            tarjan(v), low[u] = std::min(low[u], low[v]);
        else if (insta[v])
            low[u] = std::min(low[u], dfn[v]);
    }
    if (dfn[u] == low[u]) {
        do present[sta[sta[0]]] = u, insta[sta[sta[0]]] = false;
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
    
    head[0] = ts = 0;
    for (Iter i = beg; i != end; ++i) {
        head[U(i)] = dfn[U(i)] = 0, present[U(i)] = -1;
        head[V(i)] = dfn[V(i)] = 0, present[V(i)] = -1;
    }
    
    for (Iter i = beg; i != midth; ++i) addedg(U(i), V(i));
    for (Iter i = beg; i != midth; ++i) {
        if (!dfn[U(i)]) tarjan(U(i));
        if (!dfn[V(i)]) tarjan(V(i));
    }
    
    // for (int p : pnts) printf("%d(dfn=%d,low=%d) presented as %d\n", p, dfn[p], low[p], present[p]);
    std::vector<std::tuple<int,int,int>> add2right;
    add2right.reserve(midth - beg);
    Iter realmidth = beg;
    for (Iter i = beg; i != midth; ++i) {
        assert(~present[U(i)]), assert(~present[V(i)]);
        if (present[U(i)] != present[V(i)]) // 一个连通块内点 直接暴力替换成一个相同的点就可以了
            add2right.emplace_back(present[U(i)], present[V(i)], T(i));
        else *realmidth++ = *i;
    }
    for (Iter i = midth; i != end; ++i) {
        if (~present[U(i)]) U(i) = present[U(i)];
        if (~present[V(i)]) V(i) = present[V(i)];
    }
    Iter it = realmidth;
    for (const auto &el : add2right)  *it++ = el;
    solve(l, mid, beg, realmidth), solve(mid + 1, r, realmidth, end);
}

namespace SGT {
struct SegNode {
    int ls, rs;
    int cnt;
    LL sum;
};
std::array<SegNode, N * 20> tr;
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
        tr[u].sum += tr[v].sum;
        tr[u].cnt += tr[v].cnt;
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
    if (rnk == tr[u].cnt) return tr[u].sum;
    if (l == r) return tr[u].sum / tr[u].cnt * std::min(tr[u].cnt, rnk);
    int mid = l + r >> 1;
    if (tr[tr[u].rs].cnt >= rnk) return que(tr[u].rs, mid + 1, r, rnk);
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
        fprintf(stderr, "(%lld,%d) ", tr[x].sum, tr[x].cnt);
    } else {
        debug(tr[x].ls);
        debug(tr[x].rs);
    }
}
} // namespace SGT

inline void unite(int u, int v) {
    u = getroot(u), v = getroot(v);
    if (u ^ v) {
        // fprintf(stderr, "Unite %d to %d ", u, v);
        // SGT::debug(u), fputc('+', stderr), SGT::debug(v), fputc('=', stderr);
        fa[u] = v, SGT::merge(v, u);
        // SGT::debug(v); fputc('\n', stderr);
    }
}
inline int getrnk(int s) {
    return std::lower_bound(uni + 1, uni + 1 + uni[0], s) - uni;
}

int main() {
    // freopen("P5163_t.in", "r", stdin);
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

    // for (auto it = adds.rbegin(); it != adds.rend(); ++it) { fprintf(stderr, "%d->%d when time %d\n", U(it), V(it), T(it)); }
    solve(0, q+1, adds.rbegin(), adds.rend());
    // fputs("=--=--=--=--=\n", stderr); for (auto it = adds.rbegin(); it != adds.rend(); ++it) { fprintf(stderr, "%d->%d when time %d\n", U(it), V(it), T(it)); }

    for (int i = 1; i <= n; i++) { SGT::update(i, getrnk(s[i]), s[i], 1); }
    std::reverse(ques.begin()+1, ques.end());
    auto it = adds.rbegin();
    for (int i = 0; i <= q; i++) {
        for (; it != adds.rend() && T(it) <= i; ++it) unite(U(it), V(it));
        const auto &[opt, x, y] = ques[i];
        switch (opt) {
        case 2:
            // fprintf(stderr, "Make #%d: %d --> %d\n", x, s[x], s[x] - y);
            SGT::update(x, getrnk(s[x]), -s[x], -1);
            s[x] -= y;
            SGT::update(x, getrnk(s[x]), s[x], 1);
            break;
        case 3:
            ans.push_back(SGT::query(x, y));
        }
        /* fprintf(stderr, "After oper %d (%d,%d,%d) rt%d: \n", i, opt, x, y, getroot(x));
        fprintf(stderr, "s: ");
        for (int i = 1; i <= n; i++) fprintf(stderr, "%d ", s[i]);
        fputc('\n', stderr);
        for (int i = 1; i <= n; i++) {
            if (fa[i] == i) {
                fprintf(stderr, "Group %d: ", i);
                SGT::debug(rt[i]);
                fputc('\n', stderr);
            }
        } */
    }
    for (auto it = ans.rbegin(); it != ans.rend(); ++it) printf("%lld\n", *it);
    return 0;
}