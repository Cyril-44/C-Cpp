#include <bits/stdc++.h>
using namespace std;
#define fr(i, a, b) for (int i = (a); i <= (b); ++i)
#define rp(i, a, b) for (int i = (a); i >= (b); --i)
const int N = 2005, mod = 998244353;
int n, k, fl[N], dp[N][N][3][2], tp[N][3][2];
vector<int> e[N];
inline void ad(int &u, int v) {
    u += v;
    (u >= mod && (u -= mod));
}
void dfs(int x, int fa) {
    fr(i, 0, n - 1) dp[x][i][i == 0 ? 1 : (fl[x] ? 0 : 2)][fl[x] || i == 0] = 1;
    for (int y : e[x])
        if (y != fa) {
            dfs(y, x);
            fr(j, 0, n - 1) fr(h, 0, 2) fr(l, 0, 1) if (dp[x][j][h][l]) fr(p, 0, 2) fr(q, 0, 1)
                fr(d, -1, 1) if (j + d >= 0 && j + d < n && (q || d < 0) && (p || d > 0))
                    ad(tp[j][(p == 1 && d == -1 ? 1 : (p == 0 ? 0 : 2)) == 2 || h == 2
                                 ? 2 ^ (p == 1 && d == -1 ? 1 : (p == 0 ? 0 : 2)) ^ h
                                 : (p == 1 && d == -1 ? 1 : (p == 0 ? 0 : 2)) | h][l | (d == 1)],
                       dp[x][j][h][l] * 1ll * dp[y][j + d][p][q] % mod);
            fr(j, 0, n - 1) fr(h, 0, 2) fr(l, 0, 1) dp[x][j][h][l] = tp[j][h][l], tp[j][h][l] = 0;
        }
    return;
}
int main() {
    freopen("dis.in", "r", stdin);
    freopen("dis.out", "w", stdout);
    scanf("%d%d", &n, &k);
    fr(i, 1, k) {
        int x;
        scanf("%d", &x);
        fl[x] = 1;
    }
    fr(i, 1, n - 1) {
        int x, y;
        scanf("%d%d", &x, &y);
        e[x].push_back(y);
        e[y].push_back(x);
    }
    dfs(1, 0);
    int ans = 0;
    fr(j, 0, n - 1) fr(x, 1, 2) ad(ans, dp[1][j][x][1]);
    printf("%d\n", ans);
    return 0;
}