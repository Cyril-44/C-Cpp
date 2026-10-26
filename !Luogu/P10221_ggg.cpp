#include <bits/stdc++.h>
#define lowbit(x) (x & -x)
#define For(i, a, b) for (int i = (a); i <= (b); ++i)
#define Rof(i, a, b) for (int i = (a); i >= (b); --i)
using namespace std;
const int Maxn = (1 << 15), Mod = 1e9 + 7;

inline int Pow(int x, int y) {
    int res = 1;
    while (y) {
        if (y & 1) res = 1ll * res * x % Mod;
        x = 1ll * x * x % Mod, y >>= 1;
    }
    return res;
}

int n, m, K, lim, ans;
int f[Maxn + 5], g[Maxn + 5][20], h[Maxn + 5][20];
int vis[20], fac[Maxn + 5], inv[Maxn + 5], cnt[Maxn + 5];
inline int C(int x, int y) {
    if (x < y || y < 0) return 0;
    return 1ll * fac[x] * inv[x - y] % Mod * inv[y] % Mod;
}

int main() {
    cin >> n >> m >> K;
    lim = (1 << n) - 1, f[0] = 1;
    For(i, 0, n) g[0][i] = 1, h[0][i] = Mod - 1;
    fac[0] = inv[0] = 1;
    For(i, 1, Maxn) fac[i] = 1ll * fac[i - 1] * i % Mod;
    inv[Maxn] = Pow(fac[Maxn], Mod - 2);
    Rof(i, Maxn - 1, 1) inv[i] = 1ll * inv[i + 1] * (i + 1) % Mod;
    For(i, 1, m) {
        int a, b;
        cin >> a >> b;
        vis[a] |= (1 << b - 1);
        cnt[(1 << a - 1) | (1 << b - 1)]++;
    }
    For(i, 0, n - 1) For(j, 0, lim) if (j & (1 << i)) cnt[j] += cnt[j ^ (1 << i)];
    For(i, 0, lim) For(j, 1, n) if (!(i & (1 << j - 1)) && !(vis[j] & i)) f[i | (1 << j - 1)] =
        (f[i | (1 << j - 1)] + f[i]) % Mod;
    For(i, 1, lim) {
        int w = lowbit(i);
        for (int j = i; j; j = (j - 1) & i)
            if ((j & w) && !(cnt[i] - cnt[j] - cnt[i ^ j])) {
                For(k, 0, n) h[i][k] = (h[i][k] - 1ll * f[j] * h[i ^ j][k] % Mod * k % Mod + Mod) % Mod;
            }
        for (int j = i; j; j = (j - 1) & i) {
            int k = (i ^ j), flg = 1;
            For(p, 1, n) if ((k & (1 << p - 1)) && (vis[p] & j)) {
                flg = 0;
                break;
            }
            if (flg) { For(a, 0, n) g[i][a] = (g[i][a] + 1ll * h[j][a] * g[k][a]) % Mod; }
        }
    }
    static int val[20], num[20], dx[20];
    For(i, 0, n) {
        val[i] = g[lim][i];
        For(j, 0, n) if (i != j) val[i] = 1ll * val[i] * Pow(i - j + Mod, Mod - 2) % Mod;
    }
    For(i, 0, n) {
        memset(dx, 0, sizeof(dx)), dx[0] = 1;
        For(j, 0, n) if (i != j) {
            Rof(k, n, 1) dx[k] = (dx[k - 1] - 1ll * j * dx[k] % Mod + Mod) % Mod;
            dx[0] = 1ll * (Mod - j) * dx[0] % Mod;
        }
        For(j, 0, n) num[j] = (num[j] + 1ll * dx[j] * val[i]) % Mod;
    }
    For(i, 1, n) ans = (ans + 1ll * num[i] * C(K + 1, i) % Mod * fac[i]) % Mod;
    ans = 1ll * ans * inv[n] % Mod * fac[K] % Mod;
    For(i, 1, K) ans = 1ll * ans * Pow(n + i, Mod - 2) % Mod;
    cout << ans << endl;
    return 0;
}
