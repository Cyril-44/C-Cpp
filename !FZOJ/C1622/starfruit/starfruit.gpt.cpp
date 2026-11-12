
#include <bits/stdc++.h>
using namespace std;

constexpr int N = 300005, MOD = 998244353;
constexpr int I2 = 499122177, I4 = 748683265;
constexpr int S[5] = {0, 1, 2, 4, 7};

int n, ans;
int f[N][8], to[8][8][4];
vector<int> g[N];

void init() {
    for (int a : S) for (int b : S)
        for (int k = 0; k < 4; k++) {
            int p[6] = {0, 1, 2, 3, 4, 5};
            auto add = [&](int x, int y) {
                int u = p[x], v = p[y];
                for (int i = 0; i < 6; i++)
                    if (p[i] == u) p[i] = v;
            };

            if (a & 1) add(0, 1);
            if (a & 2) add(0, 2);
            if (a & 4) add(1, 2);
            if (b & 1) add(3, 4);
            if (b & 2) add(3, 5);
            if (b & 4) add(4, 5);

            if (k & 1) add(0, 3);
            if (k & 2) add(2, 4);

            to[a][b][k] =
                (p[0] == p[1]) |
                ((p[0] == p[5]) << 1) |
                ((p[1] == p[5]) << 2);
        }
}

void dfs(int u) {
    fill(f[u], f[u] + 8, 0);

    if (g[u].empty()) {
        f[u][7] = 1;
        return;
    }

    bool fst = true;
    for (int v : g[u]) {
        dfs(v);

        if (fst) {
            fst = false;
            for (int s : S) {
                int x = 1LL * f[v][s] * I2 % MOD;
                f[u][s] = (f[u][s] + x) % MOD;
                f[u][s & 4] = (f[u][s & 4] + x) % MOD;
            }
            continue;
        }

        int x = (f[u][2] + f[u][7]) % MOD;
        int y = (f[v][1] + f[v][7]) % MOD;
        ans = (ans - I2 + 1LL * x * y % MOD * I4 % MOD + MOD) % MOD;

        int h[8] = {};
        for (int a : S) for (int b : S) {
            int z = 1LL * f[u][a] * f[v][b] % MOD * I4 % MOD;
            for (int k = 0; k < 4; k++) {
                int &w = h[to[a][b][k]];
                w = (w + z) % MOD;
            }
        }
        copy(h, h + 8, f[u]);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    init();

    int T;
    cin >> T;
    while (T--) {
        cin >> n;
        for (int i = 1; i <= n; i++) {
            g[i].clear();
            int c;
            cin >> c;
            while (c--) {
                int v;
                cin >> v;
                g[i].push_back(v);
            }
        }

        ans = 1LL * (n + 1) * I2 % MOD;
        dfs(1);
        cout << ans << '\n';
    }
}
