#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N = 5010, mod = 1e9 + 7;
ll power(ll a, ll b = mod - 2) {
    ll res = 1;
    while (b) {
        if (b & 1) res = res * a % mod;
        a = a * a % mod;
        b >>= 1;
    }
    return res;
}
ll inv[N];
int n, p, q, u, v;
ll s, t;
vector<int> G[N];
ll f[N], tf, g[N][N], tg[N];
int dep[N], sz[N];
void solve(int u, int fa) {
    f[u] = s;
    g[u][1] = t;
    sz[u] = 1;
    dep[u] = dep[fa] + 1;
    for (int v : G[u]) {
        if (v == fa) continue;
        solve(v, u);
        tf = 0;
        memset(tg, 0, sizeof(ll) * (sz[u] + sz[v] + 1));
        tf = f[u] * f[v] % mod;
        for (int i = 1; i <= sz[v]; i++) (tf += f[u] * g[v][i]) %= mod;
        for (int i = 1; i <= sz[u]; i++) {
            (tg[i] += g[u][i] * f[v]) %= mod;
            if (dep[u] & 1)
                for (int j = 1; j <= sz[v]; j++) (tg[i + j] += g[u][i] * g[v][j]) %= mod;
            else {
                ll sum = 0;
                for (int j = 1; j <= sz[v]; j++) (sum += g[v][j]) %= mod;
                (tg[i] += g[u][i] * sum) %= mod;
                for (int j = 1; j <= sz[v]; j++) (tg[i + j] += g[u][i] * (mod - g[v][j])) %= mod;
            }
        }
        f[u] = tf;
        memcpy(g[u], tg, sizeof(ll) * (sz[u] + sz[v] + 1));
        sz[u] += sz[v];
    }
    for (int i = 1; i <= sz[u]; i++) (g[u][i] *= inv[i]) %= mod;
}
int main() {
    scanf("%d%d%d", &n, &p, &q);
    if (p >= q) {
        s = 0;
        t = (q + q - p) * power(q) % mod;
    } else {
        s = (q - p) * power(q) % mod;
        t = p * power(q) % mod;
    }
    inv[1] = 1;
    for (int i = 2; i <= n; i++) inv[i] = (mod - mod / i) * inv[mod % i] % mod;
    for (int i = 1; i < n; i++) {
        scanf("%d%d", &u, &v);
        G[u].push_back(v);
        G[v].push_back(u);
    }
    solve(1, 0);
    ll ans = f[1];
    for (int i = 1; i <= n; i++) (ans += g[1][i]) %= mod;
    printf("%lld", ans);
    return 0;
}
