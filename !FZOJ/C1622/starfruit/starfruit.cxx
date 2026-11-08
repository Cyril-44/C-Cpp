#include <bits/stdc++.h>
#define ll long long
using namespace std;
const int mod = 998244353, inv2 = (mod + 1) / 2, inv4 = (ll)inv2 * inv2 % mod;
const int tp[5][3] = {{0, 1, 2}, {0, 0, 2}, {0, 1, 0}, {0, 1, 1}, {0, 0, 0}}, tc[5] = {3, 2, 2, 2, 1};
int t, n, fa[300005], rt, fc[5][2], mc[5][5][4], md[5][5][4], cl[6];
ll f[300005][5], h[300005][5];
vector<int> g[300005];
void addc(int x, int y, int k) {
    for (int i = 0; i < 3; i++) { cl[x + i] = k + tp[y][i]; }
}
void add(int x, int y) {
    if (cl[x] == cl[y]) return;
    int xx = cl[x], yy = cl[y];
    for (int i = 0; i < 6; i++) {
        if (cl[i] == yy) cl[i] = xx;
    }
}
int ID(int x, int y, int z) {
    if (x == y && y == z) return 4;
    if (x == y) return 1;
    if (x == z) return 2;
    if (y == z) return 3;
    return 0;
}
void init() {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 6; k++) cl[k] = k;
            addc(3, i, 3);
            if (j) add(0, 3);
            fc[i][j] = ID(cl[0], cl[4], cl[5]);
        }
    }
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            for (int k = 0; k < 4; k++) {
                for (int x = 0; x < 6; x++) cl[x] = x;
                addc(0, i, 0);
                addc(3, j, 3);
                if (k & 1) add(0, 3);
                if (k & 2) add(2, 4);
                int tmp[6] = {0}, cnt = 0;
                for (int x = 0; x < 6; x++) {
                    if (!tmp[cl[x]]) tmp[cl[x]] = 1, cnt++;
                }
                md[i][j][k] = tc[i] + tc[j] - cnt;
                mc[i][j][k] = ID(cl[0], cl[1], cl[5]);
            }
        }
    }
}
void dfs(int x) {
    if (g[x].empty()) {
        h[x][4] = 1;
        f[x][4] = 1;
        return;
    }
    int y = g[x][0];
    dfs(y);
    ll ff[5] = {0}, hh[5] = {0};
    for (int i = 0; i < 5; i++) {
        if (!h[y][i] && !f[y][i]) continue;
        for (int j = 0; j < 2; j++) {
            hh[fc[i][j]] = (hh[fc[i][j]] + h[y][i] * inv2) % mod;
            ff[fc[i][j]] = (ff[fc[i][j]] + (f[y][i] + (1 - j + mod) * h[y][i]) % mod * inv2) % mod;
        }
    }
    for (int y : g[x]) {
        if (y == g[x][0]) continue;
        dfs(y);
        ll ff2[5] = {0}, hh2[5] = {0};
        for (int i = 0; i < 5; i++) {
            if (!hh[i] && !ff[i]) continue;
            for (int j = 0; j < 5; j++) {
                if (!h[y][j] && !f[y][j]) continue;
                for (int k = 0; k < 4; k++) {
                    ll xx = (ff[i] * h[y][j] + hh[i] * f[y][j] - hh[i] * h[y][j] % mod * md[i][j][k] % mod + mod) %
                            mod * inv4;
                    hh2[mc[i][j][k]] = (hh2[mc[i][j][k]] + hh[i] * h[y][j] % mod * inv4) % mod;
                    ff2[mc[i][j][k]] = (ff2[mc[i][j][k]] + xx) % mod;
                }
            }
        }
        memcpy(hh, hh2, sizeof(hh));
        memcpy(ff, ff2, sizeof(ff));
    }
    memcpy(h[x], hh, sizeof(h[x]));
    memcpy(f[x], ff, sizeof(f[x]));
}
int main() {
    init();
    cin >> t;
    while (t--) {
        cin >> n;
        memset(fa, 0, sizeof(fa));
        memset(f, 0, sizeof(f));
        memset(h, 0, sizeof(h));
        for (int i = 1; i <= n; i++) { g[i].clear(); }
        for (int i = 1; i <= n; i++) {
            int k, x;
            scanf("%d", &k);
            for (int j = 1; j <= k; j++) {
                scanf("%d", &x);
                g[i].push_back(x);
                fa[x] = i;
            }
        }
        for (int i = 1; i <= n; i++) {
            if (!fa[i]) rt = i;
        }
        dfs(rt);
        ll ans = 0;
        for (int i = 0; i < 5; i++) { ans = (ans + f[rt][i]) % mod; }
        printf("%lld\n", ans);
    }
    return 0;
}