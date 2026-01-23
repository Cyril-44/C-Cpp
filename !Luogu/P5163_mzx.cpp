#include <bits/stdc++.h>
#define int long long
#define ffor(i, a, b) for (int i = (a); i <= (b); i++)
#define roff(i, a, b) for (int i = (a); i >= (b); i--)
using namespace std;
const int MAXN = 500000 + 10;
int n, m, q, lsh[MAXN], tot, s[MAXN], rt[MAXN];
map<pair<int, int>, int> qid;
struct EdgeNode {
    int x, y, t;
} e[MAXN/* * 20 */];
int op[MAXN], a[MAXN], b[MAXN];
#define mid (l + r >> 1)
namespace DS {
struct NODE {
    int lson, rson, cnt, s;
} t[MAXN];
int tot;
queue<int> bin;
int get_node(void) {
    int ans = 0;
    if (!bin.empty())
        ans = bin.front(), bin.pop();
    else
        ans = ++tot;
    return t[ans] = {0, 0, 0, 0}, ans;
}
void update(int &u, int l, int r, int p, int v, int op) {
    if (!u) u = get_node();
    t[u].cnt += op, t[u].s += v;
    if (l != r) {
        if (p <= mid)
            update(t[u].lson, l, mid, p, v, op);
        else
            update(t[u].rson, mid + 1, r, p, v, op);
    }
    return;
}
void merge(int &u, int &v, int l, int r) {
    if (!v || !u) return u = u | v, v = 0, void();
    merge(t[u].lson, t[v].lson, l, mid), merge(t[u].rson, t[v].rson, mid + 1, r);
    return t[u].cnt += t[v].cnt, t[u].s += t[v].s, bin.push(v), v = 0, void();
}
int query(int u, int l, int r, int s) {
    if (!u || s < 0) return 0;
    if (s >= t[u].cnt) return t[u].s;
    if (l == r) return lsh[l] * s;
    return query(t[u].rson, mid + 1, r, s) + query(t[u].lson, l, mid, s - t[t[u].rson].cnt);
}
} // namespace DS
vector<pair<int, int>> add[MAXN];
vector<int> G[MAXN];
int tk, dfn[MAXN], low[MAXN], col[MAXN], in[MAXN], fa[MAXN];
vector<int> tmp;
void init(void) {
    for (auto id : tmp) col[id] = 0, G[id].clear(), in[id] = dfn[id] = 0;
    tmp.clear(), tk = 0;
    return;
}
stack<int> st;
int find(int k) {
    return (fa[k] == k) ? k : (fa[k] = find(fa[k]));
}
void tarjan(int u) {
    dfn[u] = low[u] = ++tk, in[u] = 1, st.push(u);
    for (auto v : G[u]) {
        if (!dfn[v])
            tarjan(v), low[u] = min(low[u], low[v]);
        else if (in[v])
            low[u] = min(low[u], dfn[v]);
    }
    if (dfn[u] == low[u]) {
        while (st.top() != u) col[st.top()] = u, in[st.top()] = 0, st.pop();
        col[st.top()] = u, in[st.top()] = 0, st.pop();
    }
    return;
}
void add_edge(int u, int v) {
    return G[u].push_back(v), tmp.push_back(v), tmp.push_back(u), void();
}
void solve(int l, int r, vector<int> vc) {
    if (l == r || vc.empty()) return;
    vector<int> ll, rr;
    for (auto id : vc)
        if (e[id].t <= mid)
            ll.push_back(id);
        else
            rr.push_back(id);
    init();
    for (auto id : ll) add_edge(e[id].x, e[id].y);
    for (auto id : tmp)
        if (!dfn[id]) tarjan(id);
    for (auto id : rr) {
        if (col[e[id].x]) e[id].x = col[e[id].x];
        if (col[e[id].y]) e[id].y = col[e[id].y];
    }
    vector<int> fll;
    for (auto id : ll) {
        if (col[e[id].x] == col[e[id].y])
            fll.push_back(id);
        else
            rr.push_back(id), e[id].x = col[e[id].x], e[id].y = col[e[id].y], e[id].t = mid + 1;
    }
    solve(l, mid, fll), solve(mid + 1, r, rr);
    return;
}
int calc_id(int x) {
    return lower_bound(lsh + 1, lsh + tot + 1, x) - lsh;
}
signed main() {
    ios::sync_with_stdio(false), cin.tie(0), cout.tie(0);
    cin >> n >> m >> q;
    ffor(i, 1, n) cin >> s[i], lsh[++tot] = s[i];
    ffor(i, 1, m) cin >> e[i].x >> e[i].y, qid[{e[i].x, e[i].y}] = i;
    ffor(i, 1, q) {
        cin >> op[i] >> a[i] >> b[i];
        if (op[i] == 1)
            e[qid[{a[i], b[i]}]].t = q - i + 1;
        else if (op[i] == 2)
            s[a[i]] += b[i], lsh[++tot] = s[a[i]];
    }
    sort(lsh + 1, lsh + tot + 1), tot = unique(lsh + 1, lsh + tot + 1) - lsh - 1;
    vector<int> vc;
    ffor(i, 1, m) vc.push_back(i);
    solve(0, q + 1, vc);
    ffor(i, 1, m) add[q + 1 - e[i].t].push_back({e[i].x, e[i].y});
    ffor(i, 1, n) fa[i] = i;
    ffor(i, 1, n) DS::update(rt[i], 1, tot, calc_id(s[i]), s[i], 1);
    vector<int> ans;
    roff(i, q + 1, 1) {
        for (auto pr : add[i]) {
            int x = pr.first, y = pr.second;
            if (find(x) != find(y)) DS::merge(rt[find(x)], rt[find(y)], 1, tot), fa[find(y)] = find(x);
        }
        if (i == q + 1) continue;
        if (op[i] == 2) {
            int r = find(a[i]);
            DS::update(rt[r], 1, tot, calc_id(s[a[i]]), -s[a[i]], -1);
            s[a[i]] -= b[i];
            DS::update(rt[r], 1, tot, calc_id(s[a[i]]), s[a[i]], 1);
        } else if (op[i] == 3) {
            int r = find(a[i]);
            ans.push_back(DS::query(rt[r], 1, tot, b[i]));
        }
    }
    reverse(ans.begin(), ans.end());
    for (auto id : ans) cout << id << '\n';
    return 0;
}
