#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long ll;
typedef pair<int, int> pii;
const int N = 500010;
mt19937 rnd(11451419);
ll p[N];
struct Node {
    int l, r;
    int cnt;
    ll val;
} a[N << 6];
int cnt, rt[N];
void clone(int &u) {
    a[++cnt] = a[u];
    u = cnt;
}
void pushup(int u) {
    a[u].cnt = a[a[u].l].cnt + a[a[u].r].cnt;
    a[u].val = a[a[u].l].val ^ a[a[u].r].val;
}
void update(int l, int r, int t, int &u, int x) {
    clone(u);
    if (l == r) {
        a[u].cnt = x;
        a[u].val = x * p[l];
        return;
    }
    int m = l + r >> 1;
    if (t <= m)
        update(l, m, t, a[u].l, x);
    else
        update(m + 1, r, t, a[u].r, x);
    pushup(u);
}
bool compare(int l, int r, int u, int v) {
    if (l == r) return a[u].cnt < a[v].cnt;
    int m = l + r >> 1;
    if (a[a[u].l].val == a[a[v].l].val)
        return compare(m + 1, r, a[u].r, a[v].r);
    else
        return compare(l, m, a[u].l, a[v].l);
}
int n, m, v[N];
bool cmp(int x, int y) {
    return compare(1, m, rt[x], rt[y]);
}
struct Range {
    int l, r;
} b[N];
vector<pii> op[N];
int lis[N];
ll val[N];
int main() {
    freopen("exhibition.in", "r", stdin);
    freopen("exhibition.out", "w", stdout);
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++) scanf("%d", &v[i]);
    for (int i = 1; i <= m; i++) p[i] = rnd();
    sort(v + 1, v + n + 1);
    for (int i = 1; i <= m; i++) {
        scanf("%d%d", &b[i].l, &b[i].r);
        op[b[i].l].emplace_back(i, 1);
        op[b[i].r + 1].emplace_back(i, 0);
    }
    for (int i = 1; i <= n; i++) {
        rt[i] = rt[i - 1];
        lis[i] = i;
        for (pii p : op[i]) update(1, m, p.first, rt[i], p.second);
    }
    stable_sort(lis + 1, lis + n + 1, cmp);
    for (int i = 1; i <= n; i++) val[lis[i]] = v[i];
    for (int i = 1; i <= n; i++) val[i] += val[i - 1];
    for (int i = 1; i <= m; i++) printf("%lld\n", val[b[i].r] - val[b[i].l - 1]);
    return 0;
}