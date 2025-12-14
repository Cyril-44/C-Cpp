#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
template <typename T> void in(T &x) {
    x = 0;
    char c = getchar();
    int f = 1;
    while (c < '0' || c > '9') {
        if (c == '-')
            f = -1;
        c = getchar();
    }
    while (c >= '0' && c <= '9') {
        x = x * 10 + c - '0';
        c = getchar();
    }
    x *= f;
}
int dp[2][2210][95][95], n, a[4010], qzh[4010];
int ans = -INT_MAX;
int main() {
    freopen("game5.in", "r", stdin);
    // freopen("game.out","w",stdout);
    in(n);
    for (int i = 1; i <= n; i++) {
        in(a[i]);
        qzh[i] = qzh[i - 1] + a[i];
    }
    memset(dp[1], 0x3f, sizeof(dp[1]));
    memset(dp[0], 0xc0, sizeof(dp[0]));
    for (int i = max(n / 2 - 93, 0); i <= min(n / 2 + 93, n); i++) {
        for (int j = 0; j <= 93; j++) {
            for (int k = 0; k <= 93; k++) {
                if (2 * i + j + k > n && 2 * i + j <= n) {
                    if (n - (i + j) >= 0)
                        dp[0][i][j][k] = qzh[i] - qzh[n] + qzh[n - (i + j)];
                }
                if (2 * i - j + k > n && 2 * i - j <= n) {
                    if (n - (i - j) <= n)
                        dp[1][i][j][k] = qzh[i] - qzh[n] + qzh[n - (i - j)];
                }
            }
        }
    }
    for (int i = min(n / 2 + 93, n); i >= 0; i--) {
        for (int j = 0; j <= 92; j++) {
            for (int k = 0; k <= 92; k++) {
                if (-j + k >= 0)
                    if (dp[1][i + k][-j + k][k] != 0x3f3f3f3f)
                        dp[0][i][j][k] =
                            max(dp[0][i][j][k], dp[1][i + k][-j + k][k]);
                if (-j + k + 1 >= 0)
                    if (dp[1][i + k + 1][-j + k + 1][k + 1] != 0x3f3f3f3f)
                        dp[0][i][j][k] =
                            max(dp[0][i][j][k],
                                dp[1][i + k + 1][-j + k + 1][k + 1]);
            }
        }
        for (int j = 0; j <= 92; j++) {
            for (int k = 0; k <= 92; k++) {
                if (-j + k >= 0)
                    if (dp[0][i][-j + k][k] != 0xc0c0c0c0)
                        dp[1][i][j][k] =
                            min(dp[1][i][j][k], dp[0][i][-j + k][k]);
                if (-j + k + 1 >= 0)
                    if (dp[0][i][-j + k + 1][k + 1] != 0xc0c0c0c0)
                        dp[1][i][j][k] =
                            min(dp[1][i][j][k], dp[0][i][-j + k + 1][k + 1]);
            }
        }
    }
    printf("%d", dp[0][0][0][1]);
}
