#include <bits/stdc++.h>

using namespace std;
typedef long long ll;
template <typename T> inline void in(T &x) {
    char c;
    int f = 1;
    do {
        c = getchar();
        f = c == '-' ? -1 : f;
    } while (c > '9' || c < '0');
    for (x = 0; c >= '0' && c <= '9'; c = getchar())
        x = (x << 1) + (x << 3) + (c ^ 48);
    x *= f;
}
template <typename T> inline void out(T x, char c) {
    if (x == 0) {
        putchar('0'), putchar(c);
        return;
    }
    if (x < 0)
        putchar('-'), x = -x;
    int sta[25], k = 0;
    while (x)
        sta[++k] = x % 10, x /= 10;
    while (k)
        putchar(sta[k--] + '0');
    putchar(c);
}

const int N = 2e5 + 5;
const ll oo = 1e18;
int n;
struct node {
    int a, t, w, k;
    inline bool operator<(const node &o) const { return t < o.t; }
} b[N];

inline void init() {
    in(n);
    for (int i = 1; i <= n; ++i)
        in(b[i].a), in(b[i].t), in(b[i].w), in(b[i].k);
    sort(b + 1, b + n + 1);
}

ll f[N];
struct edge {
    int lim, id;
    inline bool operator<(const edge &o) {
        return lim != o.lim ? lim < o.lim : b[id].t < b[o.id].t;
    }
} c[N];

int sx[N], sy[N];
int d[N];
int m;
vector<int> e[N];

vector<int> dd[N << 2];
struct bit {
    int lim;
    vector<ll> t;
    inline void init(int _l) {
        lim = _l + 3;
        t.resize(_l + 5, -oo);
    }
    inline void add(int x, ll y) {
        for (int i = x; i <= lim; i += (i & -i))
            t[i] = max(t[i], y);
    }
    inline ll ask(int x) {
        ll res = -oo;
        for (int i = x; i > 0; i -= (i & -i))
            res = max(res, t[i]);
        return res;
    }
} t[N << 2];
#define md ((l + r) >> 1)
#define ls (p << 1)
#define rs (p << 1 | 1)
inline void build(int p, int l, int r) {
    if (l == r) {
        for (int x : e[l])
            dd[p].push_back(sy[x]);
        sort(dd[p].begin(), dd[p].end());
        dd[p].erase(unique(dd[p].begin(), dd[p].end()), dd[p].end());
        t[p].init(dd[p].size() + 5);
        return;
    }
    build(ls, l, md), build(rs, md + 1, r);
    for (int x : dd[ls])
        dd[p].push_back(x);
    for (int x : dd[rs])
        dd[p].push_back(x);
    sort(dd[p].begin(), dd[p].end());
    dd[p].erase(unique(dd[p].begin(), dd[p].end()), dd[p].end());
    t[p].init(dd[p].size() + 5);
}
inline void update(int p, int l, int r, int x, int y, ll k) {
    int pos = lower_bound(dd[p].begin(), dd[p].end(), y) - dd[p].begin() + 1;
    t[p].add(pos, k);
    if (l < r) {
        if (x <= md)
            update(ls, l, md, x, y, k);
        else
            update(rs, md + 1, r, x, y, k);
    }
}
inline ll query(int p, int l, int r, int x, int y) {
    if (r <= x) {
        int pos = upper_bound(dd[p].begin(), dd[p].end(), y) - dd[p].begin();
        return t[p].ask(pos);
    }
    ll res = query(ls, l, md, x, y);
    if (x > md)
        res = max(res, query(rs, md + 1, r, x, y));
    return res;
}

inline void work() {
    ll ans = 0;
    for (int i = 1; i <= n; ++i)
        c[i] = {b[i].t + b[i].k, i};
    sort(c + 1, c + n + 1);
    for (int i = 1; i <= n; ++i)
        sx[i] = b[i].t - b[i].a, sy[i] = b[i].t + b[i].a, d[++m] = sx[i];
    sort(d + 1, d + m + 1);
    m = unique(d + 1, d + m + 1) - d - 1;
    for (int i = 1; i <= n; ++i)
        sx[i] = lower_bound(d + 1, d + m + 1, sx[i]) - d, e[sx[i]].push_back(i);
    build(1, 1, m);
    for (int i = 1; i <= n; ++i)
        f[i] = -oo;
    for (int i = 1, k = 1; i <= n; ++i) {
        f[i] = b[i].w;
        for (; k <= n && c[k].lim <= b[i].t; ++k)
            update(1, 1, m, sx[c[k].id], sy[c[k].id], f[c[k].id]);
        f[i] = max(f[i], b[i].w + query(1, 1, m, sx[i], sy[i]));
        ans = max(ans, f[i]);
    }
    printf("%lld\n", ans);
}

int main() {
    freopen("phigros.in", "r", stdin);
    freopen("phigros.out", "w", stdout);
    init();
    work();
    return 0;
}