

#include <bits/stdc++.h>
using namespace std;
#define pri pair<int, int>
#define mkp make_pair
template <typename T>
void in(T& n) {
    n = 0;
    char c = getchar();
    bool flag = 0;
    for (; c < '0' || c > '9'; c = getchar())
        if (c == '-')
            flag = 1;
    for (; c >= '0' && c <= '9'; c = getchar())
        (n *= 10) += (c ^ 48);
    if (flag)
        n = -n;
}
int wlsk[45];
int wltp;
template <typename T>
void out(T n, char c = 0) {
    if (n == 0) {
        putchar('0');
        if (c)
            putchar(c);
        return;
    }
    if (n < 0)
        putchar('-'), n = -n;
    while (n)
        wlsk[++wltp] = (n % 10), n /= 10;
    while (wltp)
        putchar(wlsk[wltp--] ^ 48);
    if (c)
        putchar(c);
}
void chkmx(int& x, int y) { x = max(x, y); }
const int N = 5e5 + 5;
const int M = 2e6 + 5;
const int inf = 1e9 + 7;
vector<pri> e[N];
int fa[N], d[N], top[N], s[N], ss[N];
int bot[N], sz[N], hs[N];
int dfn[N], dfns[N], tdfn = 0;
void dfs1(int u, int Fa, int D, int S) {
    fa[u] = Fa, d[u] = D, sz[u] = 1, hs[u] = 0, s[u] = S;
    for (auto each : e[u]) {
        auto v = each.first;
        auto w = each.second;
        if (v == Fa)
            continue;
        dfs1(v, u, D + 1, S + w), sz[u] += sz[v];
        if (sz[v] > sz[hs[u]])
            hs[u] = v;
    }
}
void dfs2(int u, int tp) {
    top[u] = tp, dfn[u] = ++tdfn, dfns[tdfn] = bot[u] = u;
    if (hs[u])
        dfs2(hs[u], tp), bot[u] = bot[hs[u]];
    for (auto each : e[u]) {
        auto v = each.first;
        auto w = each.second;
        if (v != fa[u] && v != hs[u])
            dfs2(v, v);
    }
}

struct mset {
    priority_queue<int> q1, q2;
    void ins(int x) { q1.push(x); }
    void del(int x) { q2.push(x); }
    int top() {
        while (!q2.empty()) {
            if (q1.top() != q2.top())
                break;
            q1.pop(), q2.pop();
        }
        return q1.empty() ? -inf : q1.top();
    }
} ms[N];

int root[N];
#define ls son[o][0]
#define rs son[o][1]
#define mid ((l + r) >> 1)

namespace sgt {
int son[M][2], L[M], R[M], mx[M][2], cc[M], totn = 0;
bool cov[M];

void pushupc(int o) {
    cov[o] = cov[ls] && cov[rs];
    cov[o] = (cov[o]) || bool(cc[o]);
}
void pushupmx(int o) {
    mx[o][0] = max(mx[ls][0], mx[rs][0]);
    mx[o][1] = max(mx[ls][1], mx[rs][1]);
}

int build(int l, int r) {
    int o = ++totn;
    L[o] = l, R[o] = r, cc[o] = 0, cov[o] = 0;
    if (l == r) {
        int u = dfns[l];
        mx[o][0] = ms[u].top() + s[u];
        mx[o][1] = ms[u].top() - s[u];
        return o;
    }
    ls = build(l, mid), rs = build(mid + 1, r);
    pushupc(o), pushupmx(o);
    return o;
}
void cover(int o, int lt, int rt, bool op) {
    int l = L[o], r = R[o];
    if (l >= lt && r <= rt) {
        cc[o] += (op ? 1 : -1);
        pushupc(o);
        return;
    }
    if (lt <= mid)
        cover(ls, lt, rt, op);
    if (rt > mid)
        cover(rs, lt, rt, op);
    pushupc(o);
}
void ins(int o, int pos, int x, bool op) {
    int l = L[o], r = R[o];
    if (l == r) {
        int u = dfns[l];
        op ? ms[u].ins(x) : ms[u].del(x);
        mx[o][0] = ms[u].top() + s[u];
        mx[o][1] = ms[u].top() - s[u];
        return;
    }
    ins(pos <= mid ? ls : rs, pos, x, op);
    pushupmx(o);
}
int qmx0(int o, int rt) {  // K-(S-s)
    int l = L[o], r = R[o];
    if (r <= rt)
        return mx[o][0];
    int res = qmx0(ls, rt);
    if (rt > mid)
        res = max(res, qmx0(rs, rt));
    return res;
}
int qmx1(int o, int pos) {  // K-(s-S)
    int l = L[o], r = R[o];
    if (l >= pos)
        return mx[o][1];
    int res = qmx1(rs, pos);
    if (pos <= mid)
        res = max(res, qmx1(ls, pos));
    return res;
}
bool isc(int o, int lt, int rt) {
    if (cov[o])
        return 1;
    int l = L[o], r = R[o];
    if (l >= lt && r <= rt)
        return 0;
    if (lt <= mid)
        if (!isc(ls, lt, rt))
            return 0;
    if (rt > mid)
        if (!isc(rs, lt, rt))
            return 0;
    return 1;
}
}  // namespace sgt
#undef ls
#undef rs
#undef mid
int Qmx(int u) {
    if (!u)
        return -inf;
    int mx = 0;
    chkmx(mx, sgt::qmx0(root[u], dfn[u]) - s[u]);
    chkmx(mx, sgt::qmx1(root[u], dfn[u]) + s[u]);
    return mx;
}
int n, m, q;
void init() {
    in(n), in(m), in(q);
    for (int i = 1, u, v, w; i < n; i++) {
        in(u), in(v), in(w);
        e[u].emplace_back(mkp(v, w));
        e[v].emplace_back(mkp(u, w));
    }
    dfs1(1, 0, 0, 0), dfs2(1, 1);
    for (int i = 1; i <= n; i++) {
        ss[dfn[i]] = s[i];
        int u = dfns[i];
        root[u] = root[top[u]];
        if (u == top[u])
            root[u] = sgt::build(dfn[u], dfn[bot[u]]);
    }
}
void ins(int u, int K, bool op) {
    while (u && K >= 0) {
        int tp = top[u], bt = bot[u], rt = root[u], md = dfn[u];
        sgt::ins(rt, md, K, op);
        int l = lower_bound(ss + dfn[tp], ss + dfn[bt] + 1, ss[md] - K) - ss,
            r = upper_bound(ss + dfn[tp], ss + dfn[bt] + 1, ss[md] + K) - ss - 1;
        if (l <= r)
            sgt::cover(rt, l, r, op);
        K -= (s[u] - s[fa[tp]]), u = fa[tp];
    }
}
int stkX[N], stkY[N], smxX[N], smxY[N], topx = 0, topy = 0;
bool getans(int u, int v) {
    topx = topy = 0;
    while (top[u] != top[v]) {
        if (d[top[u]] > d[top[v]]) {
            stkX[++topx] = u;
            u = fa[top[u]];
            smxX[topx] = max(0, Qmx(u));
        } else {
            stkY[++topy] = v;
            v = fa[top[v]];
            smxY[topy] = max(0, Qmx(v));
        }
    }
    if (d[u] > d[v])
        swap(u, v);
    int lca = u, mxcov = 0, lt;
    int len = (s[v] - s[fa[u]]);
    u = fa[top[u]];
    while (u)
        mxcov = max(mxcov, Qmx(u) - (s[fa[lca]] - s[u])), u = fa[top[u]];
    u = lca;
    if (mxcov < len) {
        lt = upper_bound(ss + dfn[u], ss + dfn[v] + 1, s[fa[u]] + mxcov) - ss;
        if (!sgt::isc(root[u], lt, dfn[v]))
            return 0;
    }
    chkmx(smxX[topx], mxcov - (s[fa[top[stkX[topx]]]] - s[fa[lca]]));
    chkmx(smxY[topy], mxcov - (s[fa[top[stkY[topy]]]] - s[fa[lca]]));
    while (topx) {
        u = top[stkX[topx]], v = stkX[topx];
        len = (s[v] - s[fa[u]]), mxcov = smxX[topx];
        if (mxcov < len) {
            lt = upper_bound(ss + dfn[u], ss + dfn[v] + 1, s[fa[u]] + mxcov) - ss;
            if (!sgt::isc(root[u], lt, dfn[v]))
                return 0;
        }
        topx--;
        chkmx(smxX[topx], mxcov - len);
    }
    while (topy) {
        u = top[stkY[topy]];
        v = stkY[topy];
        len = (s[v] - s[fa[u]]);
        mxcov = smxY[topy];
        if (mxcov < len) {
            lt = upper_bound(ss + dfn[u], ss + dfn[v] + 1, s[fa[u]] + mxcov) - ss;
            if (!sgt::isc(root[u], lt, dfn[v]))
                return 0;
        }
        topy--;
        chkmx(smxY[topy], mxcov - len);
    }
    return 1;
}
int P[N << 1], K[N << 1];
void work() {
    for (int i = 1; i <= m; i++)
        in(P[i]), in(K[i]), ins(P[i], K[i], 1);
    int op, x, y;
    while (q--) {
        in(op), in(x);
        if (op != 2)
            in(y);
        if (op == 1)
            ++m, P[m] = x, K[m] = y, ins(x, y, 1);
        if (op == 2)
            ins(P[x], K[x], 0);
        if (op == 3)
            getans(x, y) ? printf("Yes\n") : printf("No\n");
    }
}
int main() {
    freopen("secure.in", "r", stdin);
    freopen("secure.out", "w", stdout);
    init();
    work();
    return 0;
}
