#include <bits/stdc++.h>
#define ll long long
using namespace std;
const int mod = 1e9 + 7;
int n, op, a[105], id[105], f[105][105][105], b[105], h[105][105], ans, cnt, g[105][105][105];
ll C[105][105];
int main() {
    C[0][0] = 1;
    for (int i = 1; i <= 100; i++) {
        C[i][0] = C[i][i] = 1;
        for (int j = 1; j < i; j++) { C[i][j] = (C[i - 1][j - 1] + C[i - 1][j]) % mod; }
    }
    cin >> n >> op;
    for (int i = 1; i <= n; i++) {
        scanf("%d", &a[i]);
        id[a[i]] = i;
    }
    for (int i = 1; i < n; i++) {
        for (int j = 1; j <= n; j++) {
            if (a[i] + 1 == a[j]) { b[a[j]] = j > i; }
        }
    }
    h[1][1] = 1;
    for (int i = 2; i <= n; i++) {
        if (b[i]) {
            for (int j = 1; j <= i; j++) {
                for (int k = 1; k < j; k++) { h[i][j] = (h[i][j] + h[i - 1][k]) % mod; }
            }
        } else {
            for (int j = 1; j <= i; j++) {
                for (int k = j; k < i; k++) { h[i][j] = (h[i][j] + h[i - 1][k]) % mod; }
            }
        }
    }
    for (int i = 1; i <= n; i++) { ans = (ans + h[n][i]) % mod; }
    if (op == 1) {
        cout << ans << endl;
        return 0;
    }
    for (int x = 1; x <= n; x++) {
        memset(g, 0, sizeof(g));
        memset(f, 0, sizeof(f));
        f[1][1][1] = 1;
        for (int i = x + 1; i <= n; i++) {
            int ii = i - x + 1;
            for (int w = 1; w < ii; w++) {
                if (b[i]) {
                    for (int j = 1; j <= ii; j++) {
                        for (int k = 1; k < j; k++) {
                            if (j <= w) {
                                f[ii][j][w + 1] = (f[ii][j][w + 1] + f[ii - 1][k][w]) % mod;
                                g[ii][j][w + 1] = (g[ii][j][w + 1] + g[ii - 1][k][w]) % mod;
                                if (id[i] > id[x]) g[ii][j][w + 1] = (g[ii][j][w + 1] + f[ii - 1][k][w]) % mod;
                            } else {
                                f[ii][j][w] = (f[ii][j][w] + f[ii - 1][k][w]) % mod;
                                g[ii][j][w] = (g[ii][j][w] + g[ii - 1][k][w]) % mod;
                                if (id[i] < id[x]) g[ii][j][w] = (g[ii][j][w] + f[ii - 1][k][w]) % mod;
                            }
                        }
                    }
                } else {
                    for (int j = 1; j <= ii; j++) {
                        for (int k = j; k < ii; k++) {
                            if (j <= w) {
                                f[ii][j][w + 1] = (f[ii][j][w + 1] + f[ii - 1][k][w]) % mod;
                                g[ii][j][w + 1] = (g[ii][j][w + 1] + g[ii - 1][k][w]) % mod;
                                if (id[i] > id[x]) g[ii][j][w + 1] = (g[ii][j][w + 1] + f[ii - 1][k][w]) % mod;
                            } else {
                                f[ii][j][w] = (f[ii][j][w] + f[ii - 1][k][w]) % mod;
                                g[ii][j][w] = (g[ii][j][w] + g[ii - 1][k][w]) % mod;
                                if (id[i] < id[x]) g[ii][j][w] = (g[ii][j][w] + f[ii - 1][k][w]) % mod;
                            }
                        }
                    }
                }
            }
        }
        for (int i = 1; i <= n - x + 1; i++) {
            ll cntt = 0;
            for (int j = 1; j <= n - x + 1; j++) { cntt = (cntt + g[n - x + 1][j][i]) % mod; }
            for (int k = 1; k <= x; k++) {
                cnt = (cnt +
                       1ll * cntt * h[x][k] % mod * C[k - 1 + i - 1][k - 1] % mod * C[n - x + 1 - i + x - k][x - k]) %
                      mod;
            }
        }
    }
    cout << ans << " " << cnt << endl;
    return 0;
}