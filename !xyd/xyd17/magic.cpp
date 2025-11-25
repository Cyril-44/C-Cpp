#include <bits/stdc++.h>
using namespace std;
// typedef long long ll;
typedef unsigned int ll;
template <typename T> void in(T &x) {
    x = 0;
    char c = getchar();
    int f = 1;
    while (c < '0' || c > '9') {
        if (c == '-')
            f = -1;
        c = getchar();
    }
    while (c >= '0' && c <= '9') {
        x = x * 10 + c - '0';
        c = getchar();
    }
    x *= f;
}
const int N = 150010;
int n, c, q, l, r, a[N], cur[N], pre[N][55], nxt[N][55];
struct SegTree {
    ll a[N << 2], d[N << 2], la[N << 2], ld[N << 2];
    int t;
#define ls (u << 1)
#define rs (u << 1 | 1)
    void pushup(int u) {
        a[u] = a[ls] + a[rs];
        d[u] = d[ls] + d[rs];
    }
    void pushdown(int l, int r, int u) {
        int m = l + r >> 1;
        a[ls] += la[u] * (m - l + 1);
        d[ls] += ld[u] * (m - l + 1);
        a[rs] += la[u] * (r - m);
        d[rs] += ld[u] * (r - m);
        la[ls] += la[u];
        ld[ls] += ld[u];
        la[rs] += la[u];
        ld[rs] += ld[u];
        la[u] = ld[u] = 0;
    }
    void update(int l, int r, int tl, int tr, int u, ll x) {
        if (tl > tr)
            return;
        if (tl <= l && r <= tr) {
            a[u] += x * (r - l + 1);
            d[u] += x * (r - l + 1) * t;
            la[u] += x;
            ld[u] += x * t;
            return;
        }
        pushdown(l, r, u);
        int m = l + r >> 1;
        if (tl <= m)
            update(l, m, tl, tr, ls, x);
        if (m + 1 <= tr)
            update(m + 1, r, tl, tr, rs, x);
        pushup(u);
    }
    ll query(int l, int r, int tl, int tr, int u) {
        if (tl > tr)
            return 0;
        if (tl <= l && r <= tr)
            return a[u] * t - d[u];
        pushdown(l, r, u);
        int m = l + r >> 1;
        ll res = 0;
        if (tl <= m)
            res += query(l, m, tl, tr, ls);
        if (m + 1 <= tr)
            res += query(m + 1, r, tl, tr, rs);
        return res;
    }
    void updtime() { t++; }
} seg;
struct Node {
    int l, r, val;
};
vector<Node> upd[N], qry[N];
ll ans[N << 1];
int main() {
    freopen("magic.in", "r", stdin);
    freopen("magic.out", "w", stdout);
    in(n);
    in(c);
    in(q);
    for (int i = 1; i <= n; i++)
        in(a[i]), cur[a[i]] = i;
    for (int i = 1; i <= q; i++) {
        in(l);
        in(r);
        qry[r].push_back((Node){l, r, i});
    }
    set<int> s;
    for (int i = n; i >= 1; i--) {
        s.insert(cur[i]);
        auto it = s.lower_bound(cur[i]);
        auto itt = it;
        pre[i][0] = nxt[i][0] = cur[i];
        for (int j = 1; j <= c; j++)
            pre[i][j] = 0, nxt[i][j] = n + 1;
        if (it != s.begin())
            for (int j = 1; j <= c; j++) {
                it--;
                pre[i][j] = *it;
                if (it == s.begin())
                    break;
            }
        it = itt;
        for (int j = 1; j <= c; j++) {
            it++;
            if (it == s.end())
                break;
            nxt[i][j] = *it;
        }
        for (int j = 0; j < c; j++) {
            upd[nxt[i][c - j - 1]].push_back(
                (Node){pre[i][j + 1] + 1, pre[i][j], i});
            upd[nxt[i][c - j]].push_back(
                (Node){pre[i][j + 1] + 1, pre[i][j], -i});
        }
    }
    for (int i = 1; i <= n; i++) {
        for (Node x : upd[i])
            seg.update(1, n, x.l, x.r, 1, x.val);
        seg.updtime();
        for (Node x : qry[i])
            ans[x.val] = seg.query(1, n, x.l, x.r, 1);
    }
    for (int i = 1; i <= q; i++)
        printf("%u\n", ans[i]);
    return 0;
}