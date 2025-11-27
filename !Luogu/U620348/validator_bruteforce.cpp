#include <bits/stdc++.h>
using namespace std;

constexpr int MOD = 998244353;
constexpr auto IN = "connector.in", OUT = "connector.ans";

int g[5][105];
int f[5][105];

int main() {
    ifstream fin(IN);
    ofstream fout(OUT);
    int T, n, m;
    fin >> T >> n;
    while (T--) {
        fin >> m;
        int prod = n * m, lmt = 1 << prod;
        auto extr = [&](int st) {
            for (int i = 1; i <= n; i++)
                for (int j = 1; j <= m; j++)
                    g[i][j] = (st >> (i-1) * m + (j-1) & 1);
        };
        auto calc = [&]() -> int {
            int tot = 1;
            f[1][1] = 1;
            for (int i = 1; i <= n; i++)
                for (int j = 1; j <= m; j++) {
                    if (i == 1 && j == 1) continue;
                    if (g[i-1][j] == g[i][j]) f[i][j] = f[i-1][j];
                    else if (g[i][j-1] == g[i][j]) f[i][j] = f[i][j-1];
                    else f[i][j] = ++tot;
                }
            return tot;
        };
        int ans = 0;
        for (int st = 0; st < lmt; ++st) {
            extr(st);
            if ((ans += calc()) >= MOD) ans -= MOD;
        }
        fout << ans << '\n';
    }
    fin.close();
    fout.close();
    return 0;
}
