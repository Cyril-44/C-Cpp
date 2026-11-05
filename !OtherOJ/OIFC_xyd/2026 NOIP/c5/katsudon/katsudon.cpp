#include <bits/stdc++.h>
constexpr int N = 2005, MOD = (int)1e9 + 7;
#ifdef CLANGD
#define N 25
#endif
uint64_t a[N];
std::vector<int> g[N];
struct Mint {
    Mint& operator+=(const Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(const Mint o) { if ((val -= o.val) < MOD) val += MOD; return *this; }
    Mint& operator*=(const Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint x, const Mint y) { return x += y; }
    friend Mint operator-(Mint x, const Mint y) { return x -= y; }
    friend Mint operator*(Mint x, const Mint y) { return x *= y; }
    Mint pow(int n) {
        Mint res = 1, b = *this;
        for (; n; n >>= 1, b *= b)
            if (n & 1) res *= b;
        return res;
    }
    Mint(int v=0) : val(v) {}
    Mint(uint64_t v) : val(v % MOD) {}
private: int val;
} f[N][N], tp[N], C[N][N];
int sz[N];
void dfs(int u, int fa) {
    for (int v : g[u]) if (v != fa) dfs(v, u);
    f[u][0] = 1;
    for (int v : g[u]) if (v != fa) {
        memset(tp, 0, sizeof tp);
        for (int i = 0; i <= sz[u]; i++)
            for (int j = 0; j <= sz[v]; j++)
                for (int k = 0; k <= i && k <= j; k++)
                    tp[i+j-k] += f[u][i] * f[v][j] * C[i+j-k][j] * C[j][k];
        memcpy(f[u], tp, sizeof tp);
        sz[u] += sz[v];
    }
    ++sz[u];
    if (u == 1) {
        for (int i = sz[u]; i >= 1; i--)
            f[u][i] = f[u][i-1];
        f[u][0] = 0;
    } else {
        for (int i = sz[u]; i >= 1; i--)
            f[u][i] += f[u][i-1];
    }
}
int main() {
    freopen("katsudon.in", "r", stdin);
    freopen("katsudon.out", "w", stdout);
    int n, m;
    scanf("%d%d", &n, &m);
    for (int i = 0; i <= n; i++) {
        C[i][0] = C[i][i] = 1;
        for (int j = 1; j < i; j++)
            C[i][j] = C[i-1][j-1] + C[i-1][j];
    }
    uint64_t opval = 0;
    for (int i = 1; i <= n; i++) {
        uint64_t ai;
        scanf("%llu", &ai);
        opval ^= ai;
    }
    for (int i = 1, u, v; i < n; i++) {
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    dfs(1, 0);
    Mint res = 0, com = 1;
    for (int i = 1; i <= n && i <= m; i++) {
        com *= Mint(i).pow(MOD-2) * (m-i+1);
        res += f[1][i] * com;
    }
    printf("%d\n", opval * res);
}