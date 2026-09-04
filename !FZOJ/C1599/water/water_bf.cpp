#include <cstdio>
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <vector>
constexpr int N = 11, K = 10, MOD = (int)1e9 + 7;
std::vector<int> sons[N];
int w[K+1], a[N];
int n, m;
struct Mint {
    Mint& operator+=(Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint x, Mint y) { return x += y; }
    friend Mint operator-(Mint x, Mint y) { return x -= y; }
    friend Mint operator*(Mint x, Mint y) { return x *= y; }
    Mint(int v=0) : val(v) {}
    int val;
} C[N][N], pw2[N];
namespace Sub {
    int sz[N];
    Mint f[N][1<<K][N][N], g[1<<K][N][N];
    Mint ans;
    void dfs(int u, int fa) {
        f[u][a[u]][0][0] = 1;
        sz[u] = 1;
        for (int v : sons[u]) if (v != fa) {
            dfs(v, u);
            memcpy(g, f[u], sizeof g);
            for (int su = 0; su < (1<<m); su++)
                for (int sv = 0; sv < (1<<m); sv++)
                    for (int xu = 0; xu < sz[u]; xu++)
                        for (int xv = 0; xv < sz[v]; xv++)
                            for (int yu = 0; yu <= xu+1; yu++)
                                for (int yv = 1; yv <= xv+1; yv++)
                                    g[su | sv][xu + xv + 1][yu + yv] += f[u][su][xu][yu] * f[v][sv][xv][yv];
            sz[u] += sz[v];
            memcpy(f[u], g, sizeof g);
        }
        for (int s = 1; s < (1<<m); s++) {
            for (int x = 1; x < sz[u]; x++)
                for (int y = 1; y <= x; y++) {
                    if (y == 1) 
                        for (int i = y+1; i <= x+1; i++)
                            ans += f[u][s][x][y] * w[__builtin_popcount(s)] * C[x-y][i-y-1] * pw2[n-i];
                    else 
                        for (int i = y; i <= x+1; i++)
                            ans += f[u][s][x][y] * w[__builtin_popcount(s)] * C[x+1-y][i-y] * pw2[n-i];
                }
        }
        ans += w[__builtin_popcount(a[u])] * pw2[n-1];
        f[u][a[u]][0][1] += 1;
    }
};
int main() {
    scanf("%d%d", &n, &m);
    for (int i = 0; i <= n; i++) {
        C[i][0] = C[i][i] = 1;
        for (int j = 1; j < i; j++)
            C[i][j] = C[i-1][j-1] + C[i-1][j];
    }
    pw2[0] = pw2[1] = 1;
    for (int i = 2; i <= n; i++) pw2[i] = pw2[i-1] + pw2[i-1];
    for (int i = 1; i <= m; i++) scanf("%d", &w[i]);
    for (int i = 1; i <= n; i++) {
        int l, x; scanf("%d", &l);
        while (l--) scanf("%d", &x), a[i] |= 1 << x-1;
    }
    if (n == 1) {
        printf("%d\n", w[__builtin_popcount(a[1])]);
        return 0;
    }
    for (int i = 1, u, v; i < n; i++) {
        scanf("%d%d", &u, &v);
        sons[u].push_back(v);
        sons[v].push_back(u);
    }
    Sub::dfs(1, 0);
    printf("%d\n", Sub::ans);
    return 0;
}