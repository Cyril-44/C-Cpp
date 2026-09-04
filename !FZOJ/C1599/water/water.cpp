#include <cstdio>
#include <vector>
#ifdef CLANGD
constexpr int N = 54;
#else
constexpr int N = 5004;
#endif
constexpr int K = 10, MOD = (int)1e9 + 7;
std::vector<int> sons[N];
int w[15], a[N];
struct Mint {
    Mint& operator+=(Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint x, Mint y) { return x += y; }
    friend Mint operator-(Mint x, Mint y) { return x -= y; }
    friend Mint operator*(Mint x, Mint y) { return x *= y; }
    Mint(int v=0) : val(v) {}
    int val;
} C[N][N], bell[N], g[N], f[1<<K];
bool vis[N];
int n, m;
int dfs(int u) {
    vis[u] = true; int sz = 1;
    for (int v : sons[u]) if (!vis[v]) sz += dfs(v);
    return sz;
}
int main() {
    scanf("%d%d", &n, &m);
    for (int i = 0; i <= n; i++) {
        C[i][0] = C[i][i] = 1;
        for (int j = 1; j < i; j++)
            C[i][j] = C[i-1][j-1] + C[i-1][j];
    }
    bell[0] = 1;
    for (int i = 1; i <= n; i++)
        for (int j = 0; j < i; j++)
            bell[i] += bell[j] * C[i-1][j];
    for (int i = 1; i <= m; i++)
        scanf("%d", &w[i]);
    for (int i = 1; i <= n; i++) {
        int l, x; scanf("%d", &l);
        while (l--) scanf("%d", &x), a[i] |= 1 << x-1;
    }
    for (int i = 1, u, v; i < n; i++) {
        scanf("%d%d", &u, &v);
        sons[u].push_back(v);
        sons[v].push_back(u);
    }
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= i; j++)
            g[i] += C[i][j] * bell[n - j];
    for (int s = 1; s < (1<<m); s++) {
        for (int i = 1; i <= n; i++)
            vis[i] = (a[i] & s) != a[i];
        for (int i = 1; i <= n; i++)
            if (!vis[i]) f[s] += g[dfs(i)];
    }
    for (int i = 0; i < m; i++)
        for (int s = 1; s < (1<<m); s++)
            if (s >> i & 1) f[s] -= f[s ^ (1<<i)];
    Mint ans = 0;
    for (int s = 1; s < (1<<m); s++)
        ans += f[s] * w[__builtin_popcount(s)];
    printf("%d\n", ans);
    return 0;
}