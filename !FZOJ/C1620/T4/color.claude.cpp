#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll MOD = 998244353;

ll qpow(ll a, ll b) { ll r = 1; a %= MOD; for (; b; b >>= 1, a = a * a % MOD) if (b & 1) r = r * a % MOD; return r; }

int n;
ll c, fac[2005], ifac[2005], suf[2010], P[2005];
vector<int> g[2005];

ll C(int a, int b) { return fac[a] * ifac[b] % MOD * ifac[a - b] % MOD; }

int main() {
    scanf("%d %lld", &n, &c);
    for (int i = 1, u, v; i < n; i++) scanf("%d %d", &u, &v), g[u].push_back(v), g[v].push_back(u);
    c %= MOD;
    ll d = (c + 1) % MOD;
    fac[0] = 1;
    for (int i = 1; i <= n; i++) fac[i] = fac[i - 1] * i % MOD;
    ifac[n] = qpow(fac[n], MOD - 2);
    for (int i = n; i; i--) ifac[i - 1] = ifac[i] * i % MOD;
    P[0] = 1;
    for (int i = 1; i <= n; i++) P[i] = P[i - 1] * d % MOD;
    for (int t = n; t >= 0; t--) suf[t] = (suf[t + 1] + C(n, t)) % MOD;

    ll p2 = qpow(2, n), sumcnt = 0, pr = 0;
    vector<int> par(n + 1), dep(n + 1), top(n + 1), sz(n + 1), ord;
    for (int p = 1; p <= n; p++) {
        ord.assign(1, p); par[p] = dep[p] = 0;
        for (size_t i = 0; i < ord.size(); i++) {
            int x = ord[i]; sz[x] = 1;
            for (int y : g[x]) if (y != par[x]) {
                par[y] = x; dep[y] = dep[x] + 1; top[y] = (x == p ? y : top[x]);
                ord.push_back(y);
            }
        }
        for (int i = n - 1; i >= 1; i--) sz[par[ord[i]]] += sz[ord[i]];
        ll cnt = p2;                                   // v=p 是重心的染色数
        for (int y : g[p]) cnt -= suf[n - sz[y] + 1];
        sumcnt += ((cnt - 1) % MOD + MOD) % MOD;       // 去掉全白
        for (int q = p + 1; q <= n; q++) {
            int sp = n - sz[top[q]], sq = sz[q];
            pr = (pr + P[n - 1 - dep[q]] * ((C(sp + sq, sp) - 1 + MOD) % MOD)) % MOD;
        }
    }
    sumcnt %= MOD;
    ll ans = 1 + (p2 - 1 + MOD) % MOD * P[n] % MOD
           - c * P[n - 1] % MOD * sumcnt % MOD
           + c * c % MOD * pr % MOD;
    printf("%lld\n", ((ans % MOD) + MOD) % MOD);
}