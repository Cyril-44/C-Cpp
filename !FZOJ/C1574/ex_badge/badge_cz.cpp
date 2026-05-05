#pragma GCC optimize(2)
#pragma GCC optimize(3)
#pragma GCC optimize("Ofast")
#include <bits/stdc++.h>
using namespace std;
// 边不交路径？
// 拆成nk个点总共n+2m个点，(u,i)只连upt>=t[i]的出边
mt19937 rnd(19260817);
typedef long long ll;
const int N = 1e5 + 10, mod = 1e9 + 7;
int n, m, q, lim, cntn, nid, a[N << 2][50], b[50], du[N << 2], id[N], f[50][50], g[50];
ll val[N];
bool vis[N];
int tot, fi[N << 2], ne[N * 51], to[N * 51];
ll ans;
void adde(int x, int y) {
    to[++tot] = y;
    ne[tot] = fi[x];
    fi[x] = tot;
    du[y]++;
}
void add(int &x, int y) {
    x += y;
    if (x >= mod) x -= mod;
}
queue<int> q1;
vector<int> vt[N];
int kpow(int t1, int t2) {
    int res = 1;
    while (t2) {
        if (t2 & 1) res = 1ll * res * t1 % mod;
        t1 = 1ll * t1 * t1 % mod;
        t2 >>= 1;
    }
    return res;
}
bool inst(int p) {
    for (int i = 0; i < nid; i++)
        if (b[i]) {
            if (!g[i]) {
                g[i] = p;
                memcpy(f[i], b, sizeof(b));
                return 1;
            }
            int d = 1ll * b[i] * kpow(f[i][i], mod - 2) % mod;
            for (int j = i; j < nid; j++) add(b[j], mod - 1ll * f[i][j] * d % mod);
        }
    return 0;
}
int main() {
    // freopen("badge_6.in","r",stdin);
    // freopen("badge.out","w",stdout);
    scanf("%d%d%d%d", &n, &m, &q, &lim);
    for (int i = 1; i <= n; i++) scanf("%lld", &val[i]), vt[i].push_back(i);
    cntn = n;
    for (int u, v, i = 1; i <= m; i++) {
        scanf("%d%d", &u, &v);
        int bu = ++cntn, bv = ++cntn;
        if (u == 1) adde(1, bu), du[bu] = 0;
        if (v == 1) adde(1, bv), du[bv] = 0;
        if (u != 1)
            for (int x : vt[u]) adde(bv, x);
        if (v != 1)
            for (int x : vt[v]) adde(bu, x);
        if (u != 1 && v != 1) vt[u].push_back(bu);
        if (v != 1 && u != 1) vt[v].push_back(bv);
    }
    for (int i = fi[1]; i; i = ne[i]) a[to[i]][nid++] = 1;
    for (int i = 1; i <= cntn; i++)
        if (!du[i]) q1.push(i);
    while (!q1.empty()) {
        int u = q1.front();
        q1.pop();
        for (int i = fi[u]; i; i = ne[i]) {
            int v = to[i], w = rnd() % mod;
            for (int j = 0; j < nid; j++) add(a[v][j], 1ll * a[u][j] * w % mod);
            if (!--du[v]) q1.push(v);
        }
    }
    for (int i = 2; i <= n; i++) id[i] = i;
    sort(id + 2, id + n + 1, [&](int x, int y) { return val[x] > val[y]; });
    for (int i = 2; i <= n; i++) {
        for (int j = 0; j < nid; j++) b[j] = a[id[i]][j];
        if (inst(id[i])) ans += val[id[i]];
    }
    printf("%lld\n", ans + val[1]);
    while (q--) {
        int u, v;
        scanf("%d%d", &u, &v);
        ll tmp = val[u];
        val[u] += v;
        vector<int> vec;
        vec.push_back(u);
        for (int i = 0; i < nid; i++)
            if (val[g[i]] < val[u] || g[i] == u) {
                if (g[i] != u)
                    ans -= val[g[i]];
                else
                    ans -= tmp;
                if (g[i] != u) vec.push_back(g[i]);
                g[i] = 0;
            }
        sort(vec.begin(), vec.end(), [&](int x, int y) { return val[x] > val[y]; });
        for (int x : vec) {
            for (int i = 0; i < nid; i++) b[i] = a[x][i];
            if (inst(x)) ans += val[x];
        }
        printf("%lld\n", ans + val[1]);
    }
}
