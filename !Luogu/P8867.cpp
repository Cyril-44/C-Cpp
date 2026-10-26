#include <bits/stdc++.h>
constexpr int N = 500005, MOD = (int)1e9 + 7;
#ifdef CLANGD
#define N 55
#endif
struct Mint {
    Mint& operator+=(const Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(const Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(const Mint o) { val = 1ull * val * o.val % MOD; return *this;  }
    friend Mint operator+(Mint x, const Mint y) { return x += y; }
    friend Mint operator-(Mint x, const Mint y) { return x -= y; }
    friend Mint operator*(Mint x, const Mint y) { return x *= y; }
    Mint pow(int n) {
        Mint b = *this, res = 1;
        for (; n; n >>= 1, b *= b) if (n & 1) res *= b;
        return res;
    }
    Mint(int v=0) : val(v % MOD) {}
private: int val;
} f[N];
std::vector<int> e[N], g[N];
int dfn[N], low[N];
void dfs(int u, int fa) {
    low[u] = dfn[u] = ++dfn[0];
    for (int &v : e[u]) if (v != fa) {
        if (!dfn[v]) {
            dfs(v, u), low[u] = std::min(low[u], low[v]);
            if (dfn[u] < low[v]) v = -v;
        } else if (v != fa)
            low[u] = std::min(low[u], dfn[v]);
    }
}
int id[N], cnt[N], sz[N];
void find(int u) {
    id[u] = id[0], ++cnt[id[0]];
    for (int v : e[u]) if (v > 0 && !id[u]) find(v);
}
void dp(int u, int fa) {
    sz[u] = 1;
    f[u] = Mint(2).pow(cnt[u]);
    for (int v : g[u]) if (u != fa) {
        dp(v, u);
        sz[u] += sz[v];
        f[u] *= (f[v] + Mint(2).pow(sz[v]));
    }
}
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int i = 1, u, v; i <= m; i++) {
        scanf("%d%d", &u, &v);
        e[u].push_back(v);
        e[v].push_back(u);
    }
    dfs(1, 0);
    for (int i = 1; i <= n; i++) if (!id[i]) ++id[0], find(i);
    for (int i = 1; i <= n; i++)
        for (int j : e[i]) if (j < 0)
            g[id[i]].push_back(id[-j]);
    Mint ans = 0;
    dp(1, 0);
    for (int i = 1; i <= n; i++)
        ans += f[i] * Mint(2).pow(m - sz[i]);
    printf("%d\n", ans);
    return 0;
}