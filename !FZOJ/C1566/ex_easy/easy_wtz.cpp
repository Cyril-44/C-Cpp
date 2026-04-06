#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int N = 305, Mod = 1e9 + 7;
vector<int> gr[N];
ll b[N << 1], val[N][N];
ll dp[N][N][N], f[N][N], g[N][N], p[N][N];
int n, m, x, y;
void solve(int u) {
    // printf("solve %d\n",u);
    for (int v : gr[u]) solve(v);
    for (int x = 1; x <= m; x++) {
        p[x][0] = 0;
        for (int y = 1; y <= m; y++) p[x][y] = 1;
    }
    for (int v : gr[u]) {
        for (int x = 1; x <= m; x++) {
            for (int y = 1; y <= m; y++) (p[x][y] *= dp[v][x][y]) %= Mod;
        }
    }
    memset(f, 0, sizeof(f));
    memset(g, 0, sizeof(g));
    for (int x = 1; x <= m; x++) {
        for (int y = 1; y <= m; y++) {
            ll v = (p[x][y] - p[x][y - 1] + Mod) % Mod;
            (f[x][max(y, x)] += v * val[x][max(y, x)]) %= Mod;
            if (y < x) {
                (g[x][x - 1] += v) %= Mod;
                (g[x][y] += Mod - v) %= Mod;
                (dp[u][x][y] += y * v % Mod * val[x][y]) %= Mod;
            } else
                (dp[u][x][y] += (x - 1) * v % Mod * val[x][y]) %= Mod;
        }
    }
    for (int x = m; x >= 1; x--) {
        for (int y = 1; y <= m; y++) {
            (f[x][y] += f[x + 1][y]) %= Mod;
            (dp[u][x][y] += f[x][y]) %= Mod;
        }
    }
    for (int x = 1; x <= m; x++) {
        for (int y = m; y >= 1; y--) {
            (g[x][y] += g[x][y + 1]) %= Mod;
            (dp[u][x][y] += g[x][y] * val[x][y]) %= Mod;
        }
    }
    for (int x = 1; x <= m; x++) {
        for (int y = 1; y <= m; y++) (dp[u][x][y] += dp[u][x][y - 1]) %= Mod;
    }
}
int main() {
    scanf("%d%d%d%d", &n, &m, &x, &y);
    for (int i = 1; i <= (m << 1); i++) scanf("%lld", &b[i]);
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= m; j++) { val[i][j] = b[i * x + j * y]; }
    }
    for (int i = 2; i <= n; i++) {
        int fa;
        scanf("%d", &fa);
        gr[fa].push_back(i);
    }
    solve(1);
    printf("%lld\n", dp[1][1][m]);
    return 0;
}