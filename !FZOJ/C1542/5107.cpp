#include <bits/stdc++.h>
using namespace std;

using LL = long long;
constexpr int N = 25;

LL w[N][N], wsum[N][1<<13];
LL W[1<<13], f[N], dp[100005];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m, s;
    cin >> n >> m >> s;

    for (int i = 0; i < m; ++i) {
        int x, y; long long wi;
        cin >> x >> y >> wi;
        --x; --y;
        w[x][y] += wi;
        w[y][x] += wi;
    }

    int N = 1 << n;

    for (int v = 0; v < n; ++v) {
        for (int k = 1; k < N; ++k) {
            int p = __builtin_ctz(k); 
            int dig = k ^ (1 << p);
            wsum[v][k] = wsum[v][dig] + w[v][p];
        }
    }

    memset(f, 0x80, sizeof f);
    f[0] = 0; 

    for (int k = 1; k < N; ++k) {
        int v = __builtin_ctz(k);
        int T = k ^ (1 << v);
        W[k] = W[T] + wsum[v][T];
    }

    for (int k = 0; k < N; ++k) {
        int t = __builtin_popcount(k);
        f[t] = max(f[t], W[k]);
    }

    for (int x = 1; x <= s; ++x) {
        auto &dpx = dp[x];
        for (int t = 1; t <= min(n, x); ++t) {
            if (f[t] != (int)0x80808080) {
                dpx = max(dpx, dp[x - t] + f[t]);
            }
        }
    }

    cout << dp[s] << "\n";
    return 0;
}
