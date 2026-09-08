// 题解有个地方把并集打成交集还被 N_z_ 发现了，感觉比较列文虎克
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <queue>
#include <vector>
#define mod 998244353
#define lwb __builtin_ctz
#define popc __builtin_popcount
using namespace std;
const int N = 16, M = 35, K = 50005;
int n, m, k, cnt, fac[M], inv[M], in[N], In[K], f[K], g[K][M], F[K], G[K], y[M], co[M], nw[M], sm[M], ans;
inline void add(int &x, int y) {
    x += y;
    if (x >= mod) x -= mod;
}
inline int C(int n, int m) {
    return n < m || m < 0 ? 0 : 1ll * fac[n] * inv[m] % mod * inv[n - m] % mod;
}
int qpow(int bse, int p) {
    int ans = 1;
    for (; p; p >>= 1, bse = 1ll * bse * bse % mod)
        if (p & 1) ans = 1ll * ans * bse % mod;
    return ans;
}
int solve(int x) {
    for (int s = 1; s < 1 << n; s++) {
        G[s] = F[s] = 0;
        for (int i = 1, px = 1, tp; i <= n; i++)
            px = 1ll * px * x % mod, tp = 1ll * px * g[s][i] % mod, add(G[s], i & 1 ? tp : mod - tp);
    }
    F[0] = 1;
    for (int s = 1; s < 1 << n; s++)
        for (int t = s; t; t--, t &= s)
            if (!(In[t] & (s ^ t))) add(F[s], 1ll * F[s ^ t] * G[t] % mod);
    return F[(1 << n) - 1];
}
int main() {
    freopen("timeline.in", "r", stdin);
    freopen("timeline.out", "w", stdout);
    scanf("%d%d%d", &n, &m, &k), fac[0] = inv[0] = inv[1] = co[0] = f[0] = g[0][0] = 1;
    for (int i = 1; i < M; i++) fac[i] = 1ll * i * fac[i - 1] % mod;
    for (int i = 2; i < M; i++) inv[i] = 1ll * inv[mod % i] * (mod - mod / i) % mod;
    for (int i = 1; i < M; i++) inv[i] = 1ll * inv[i] * inv[i - 1] % mod;
    for (int u, v; m--;) scanf("%d%d", &u, &v), u--, v--, in[v] |= 1 << u;
    m = n + 1;
    for (int s = 1, u; s < 1 << n; s++) u = lwb(s), In[s] = In[s ^ 1 << u] | in[u];
    for (int s = 1; s < 1 << n; s++)
        for (int i = 0; i < n; i++)
            if (s >> i & !(in[i] & s)) add(f[s], f[s ^ 1 << i]);
    for (int i = 1, u; i <= n; i++)
        for (int s = 0; s < 1 << n; s++)
            if (popc(s) >= i) {
                u = lwb(s);
                for (int t = s; t; t--, t &= s)
                    if (t >> u & !(In[t] & (s ^ t)) & !(In[s ^ t] & t))
                        add(g[s][i], 1ll * g[s ^ t][i - 1] * f[t] % mod);
            }
    for (int x = 1; x <= m; x++) y[x] = solve(x);
    for (int i = 1; i <= m; i++)
        for (int j = i; j >= 0; j--) co[j] = (1ll * co[j] * (mod - i) + (j ? co[j - 1] : 0)) % mod;
    for (int i = 1; i <= m; i++) {
        int now = 1;
        for (int j = 1; j <= m; j++)
            if (i != j) now = 1ll * now * (i - j + mod) % mod;
        now = 1ll * qpow(now, mod - 2) * y[i] % mod;
        int iv = qpow(mod - i, mod - 2);
        for (int j = 0; j <= m; j++) nw[j] = 1ll * (co[j] - (j ? nw[j - 1] : 0) + mod) * iv % mod;
        for (int j = 0; j <= m; j++) sm[j] = (sm[j] + 1ll * nw[j] * now) % mod;
    }
    for (int i = 1; i <= k + 1; i++) add(ans, 1ll * sm[i] * fac[k + 1] % mod * fac[k] % mod * inv[k + 1 - i] % mod);
    printf("%d", 1ll * ans * inv[n + k] % mod);
}