#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef unsigned long long ull;
constexpr int N = 2005;
constexpr ll MOD = 1000000007;

ll qpow(ll b, ll e) {
    ll r = 1;
    b %= MOD;
    for (; e; e >>= 1, b = b * b % MOD)
        if (e & 1) r = r * b % MOD;
    return r;
}

int n, par[N], ord[N];
ll m, G[N], P[N], y[N], fac[N], ifac[N], pre[N], suf[N];
vector<int> g[N];

int main() {
    freopen("katsudon.in", "r", stdin);
    freopen("katsudon.out", "w", stdout);
    scanf("%d%lld", &n, &m);
    ull xr = 0;  // 每个合法方案的权值都等于 a_1 ^ ... ^ a_n
    for (int i = 1; i <= n; i++) {
        ull a;
        scanf("%llu", &a);
        xr ^= a;
    }
    for (int i = 1, u, v; i < n; i++) {
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
        g[v].push_back(u);
    }

    // BFS 序,倒着扫就是先儿子后父亲
    int hd = 0, tl = 0;
    ord[tl++] = 1;
    while (hd < tl) {
        int u = ord[hd++];
        for (int v : g[u])
            if (v != par[u]) par[v] = u, ord[tl++] = v;
    }

    // G[u] 维护 sum_{t<x} P_u(t) ;  P_u(x) = prod_{c} g_c(x)
    // 非根: g_u(x) = sum_{t<=x} P_u(t);  根: g_1(x) = sum_{t<x} P_1(t)
    for (int x = 0; x <= n; x++) {
        for (int i = 1; i <= n; i++) P[i] = 1;
        for (int i = n - 1; i >= 0; i--) {
            int u = ord[i];
            if (u == 1) {
                y[x] = G[1];
                G[1] = (G[1] + P[1]) % MOD;
            } else {
                G[u] = (G[u] + P[u]) % MOD;
                P[par[u]] = P[par[u]] * G[u] % MOD;
            }
        }
    }

    // g_1(x) 是 x 的 n 次多项式,已知 x=0..n 的值,插值求 x=m
    ll ans;
    if (m <= n) {
        ans = y[m];
    } else {
        fac[0] = 1;
        for (int i = 1; i <= n; i++) fac[i] = fac[i - 1] * i % MOD;
        ifac[n] = qpow(fac[n], MOD - 2);
        for (int i = n; i > 0; i--) ifac[i - 1] = ifac[i] * i % MOD;
        pre[0] = 1;
        for (int i = 0; i <= n; i++) pre[i + 1] = pre[i] * ((m - i) % MOD) % MOD;
        suf[n + 1] = 1;
        for (int i = n; i >= 0; i--) suf[i] = suf[i + 1] * ((m - i) % MOD) % MOD;
        ans = 0;
        for (int i = 0; i <= n; i++) {
            ll t = y[i] * pre[i] % MOD * suf[i + 1] % MOD * ifac[i] % MOD * ifac[n - i] % MOD;
            if ((n - i) & 1) ans = (ans - t + MOD) % MOD;
            else ans = (ans + t) % MOD;
        }
    }
    printf("%lld\n", (ll)(xr % MOD) * ans % MOD);
}