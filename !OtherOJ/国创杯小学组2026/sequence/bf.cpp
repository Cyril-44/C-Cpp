#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) {
        int n;
        cin >> n;
        vector<int> a(n + 1);
        for (int i = 1; i <= n; i++) cin >> a[i];

        // 预处理回文
        vector<vector<bool>> isPal(n + 1, vector<bool>(n + 1, false));
        for (int i = 1; i <= n; i++) isPal[i][i] = true;

        for (int len = 2; len <= n; len++) {
            for (int l = 1; l + len - 1 <= n; l++) {
                int r = l + len - 1;
                if (a[l] == a[r]) {
                    if (len == 2) isPal[l][r] = true;
                    else isPal[l][r] = isPal[l + 1][r - 1];
                }
            }
        }

        // 预处理左半非降
        vector<vector<bool>> ok(n + 1, vector<bool>(n + 1, true));
        for (int l = 1; l <= n; l++) {
            for (int r = l; r <= n; r++) {
                int mid = (l + r) / 2;
                bool good = true;
                for (int i = l; i < mid; i++) {
                    if (a[i] > a[i + 1]) {
                        good = false;
                        break;
                    }
                }
                ok[l][r] = good;
            }
        }

        // DP
        vector<int> dp(n + 1, INF);
        dp[0] = 0;

        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= i; j++) {
                if (isPal[j][i] && ok[j][i]) {
                    dp[i] = min(dp[i], dp[j - 1] + 1);
                }
            }
        }

        cout << dp[n] << "\n";
    }

    return 0;
}