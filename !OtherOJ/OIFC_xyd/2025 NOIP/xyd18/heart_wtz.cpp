#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
template <class T> void in(T &x) {
    x = 0;
    int f = 1;
    char c = getchar_unlocked();
    while (c < '0' || c > '9') {
        if (c == '-') f = -1;
        c = getchar_unlocked();
    }
    while (c >= '0' && c <= '9') {
        x = x * 10 + c - '0';
        c = getchar_unlocked();
    }
    x *= f;
}
const int N = 5020, Mod = 998244353;
vector<int> gg[N];
int n, fa;
int f[2][N][N << 1], sz[N];
int g[2][N << 1];
void work(int u) {
    f[0][u][1] = 1;
    sz[u] = 1;
    for (int v : gg[u]) {
        work(v);

        memset(g, 0, sizeof(g));
        for (int i = 0; i <= sz[u]; i++) {
            for (int j = 0; j <= sz[v]; j++) (g[0][i + j] += f[0][u][i] * (ll)f[0][v][j] % Mod) %= Mod;
            for (int j = 1; j <= sz[v]; j++) (g[0][i] += f[0][u][i] * (ll)f[0][v][j] % Mod) %= Mod;
            (g[0][i] += f[0][u][i] * (ll)f[1][v][N] % Mod) %= Mod;
        }
        for (int i = N - sz[u]; i <= N + sz[u]; i++) {
            for (int j = N - sz[v]; j <= N + sz[v]; j++) {
                if (i >= N) (g[1][i + j - N] += f[0][u][i - N] * (ll)f[1][v][j] % Mod) %= Mod;
                if (j >= N) (g[1][i + j - N] += f[1][u][i] * (ll)f[0][v][j - N] % Mod) %= Mod;
            }
            for (int j = 1; j <= sz[v]; j++) (g[1][i] += f[1][u][i] * (ll)f[0][v][j] % Mod) %= Mod;
            (g[1][i] += f[1][u][i] * (ll)f[1][v][N] % Mod) %= Mod;
        }
        sz[u] += sz[v];
        for (int i = 0; i <= sz[u]; i++) {
            f[0][u][i] = g[0][i];
            f[1][u][i + N] = g[1][i + N];
            f[1][u][-i + N] = g[1][-i + N];
        }
    }
    for (int i = 0; i <= sz[u]; i++) (f[1][u][N - i] += f[0][u][i]) %= Mod;
}
int main() {
    freopen("heart.in", "r", stdin);
    freopen("heart.out", "w", stdout);
    in(n);
    for (int i = 2; i <= n; i++) {
        in(fa);
        gg[fa].push_back(i);
    }
    work(1);
    ll ans = f[1][1][N];
    for (int i = 1; i <= sz[1]; i++) ans += f[0][1][i];
    printf("%lld", ans % Mod);
}
