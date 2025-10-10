#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        int n, k;
        cin >> n >> k;
        int tot = n * (n - 1) / 2;
        if (k < 0 || k > tot) { cout << "0\n"; continue; }
        int S = tot - k; // sum of C(bi,2) we need
        vector<vector<int>> f(n+1, vector<int>(tot+1, -1));
        f[0][0] = 0;
        for (int i = 1; i <= n; ++i) {
            for (int j = 0; j < i; ++j) {
                int len = i - j;
                int add = len * (len - 1) / 2;
                for (int v = add; v <= tot; ++v) {
                    if (f[j][v - add] != -1 && f[i][v] == -1) {
                        f[i][v] = j;
                    }
                }
            }
        }
        if (f[n][S] == -1) {
            cout << "0\n";
            continue;
        }
        vector<int> blocks;
        int curi = n, curv = S;
        while (curi > 0) {
            int j = f[curi][curv];
            int len = curi - j;
            blocks.push_back(len);
            curv -= len * (len - 1) / 2;
            curi = j;
        }
        reverse(blocks.begin(), blocks.end());
        int cur = n;
        vector<int> ans;
        for (int len : blocks) {
            int start = cur - len + 1;
            for (int x = start; x <= cur; ++x) ans.push_back(x);
            cur = start - 1;
        }
        for (int i = 0; i < (int)ans.size(); ++i) {
            if (i) cout << ' ';
            cout << ans[i];
        }
        cout << '\n';
    }
    return 0;
}
