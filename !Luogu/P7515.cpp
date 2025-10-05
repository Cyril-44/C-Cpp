#include <bits/stdc++.h>
using namespace std;
constexpr int N = 605;
int a[N][N], b[N][N];
long long f[N + N];
vector<pair<int,int>> edgs[N + N];
inline bool spfa(const int n) {
    static int q[N * N << 3], cnt[N + N];
    static bool vis[N + N];
    memset(vis, 0, sizeof vis);
    memset(cnt, 0, sizeof cnt);
    memset(f, 0x3f, sizeof f);
    int l = 1, r = 1;
    f[q[r++] = 1] = 0;
    while (l < r) {
        int u = q[l++]; vis[u] = false;
        for (const auto &[v, w] : edgs[u])
            if (f[v] > f[u] + w) {
                f[v] = f[u] + w;
                ++cnt[v];
                if (cnt[v] >= n) return false;
                if (!vis[v])
                    q[r++] = v, vis[v] = true;
            }
    }
    return true;
}
int main() {
#ifndef ONLINE_JUDGE
    freopen("P7515_16.in", "r", stdin);
    freopen("P7515.out", "w", stdout);
#endif
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T, n, m;
    for (cin >> T; T--; ) {
        cin >> n >> m;
        for (int i = 1; i < n; i++)
            for (int j = 1; j < m; j++)
                cin >> b[i][j];
        fill(edgs+1, edgs+1 + n + m, vector<pair<int,int>>());
        for (int i = 1; i <= n; i++) a[i][m] = 0;
        for (int j = 1; j <= m; j++) a[n][j] = 0;
        for (int i = n-1; i >= 1; i--)
            for (int j = m-1; j >= 1; j--)
                a[i][j] = b[i][j] - a[i+1][j] - a[i][j+1] - a[i+1][j+1];
        // Specs: row_i *= (-1)^i, col_j *= -(-1)^j
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= m; j++) {
                if (i + j & 1) { // -a_i_j <= -r_i + +c_j <= 1e6 - a_i_j
                    edgs[j + n].emplace_back(i, a[i][j]);
                    edgs[i].emplace_back(j + n, 1000000 - a[i][j]);
                } else { // -a_i_j <= r_i - c_j <= 1e6 - a_i_j
                    edgs[i].emplace_back(j + n, a[i][j]);
                    edgs[j + n].emplace_back(i, 1000000 - a[i][j]);
                }
            }
        if (spfa(n + m)) {
            puts("YES");
            for (int i = 1; i <= n; i++) {
                for (int j = 1; j <= m; j++) {
                    if (i + j & 1) a[i][j] += -f[i] + f[j + n];
                    else a[i][j] += f[i] - f[j + n];
                    printf("%d ", a[i][j]);
                }
                putchar('\n');
            }
        }
        else puts("NO");
    }
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++) {
            a[i][j] = i * j;
        }
    return 0;
}