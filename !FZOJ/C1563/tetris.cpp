#include <bits/stdc++.h>
using namespace std;
using LL = long long;
LL dp(int n, const vector<int> &pre) {
    int N = 1 << n;
    vector<LL> dp(N, 0);
    dp[0] = 1;
    for (int mask = 0; mask < N; mask++) {
        LL cur = dp[mask];
        if (!cur) continue;
        for (int v = 0; v < n; v++) {
            if (!(mask & (1 << v))) {
                if ((pre[v] & ~mask) == 0) {
                    dp[mask | (1 << v)] += cur;
                }
            }
        }
    }
    return dp[N - 1];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    vector<vector<int>> grid(12, vector<int>(8));
    map<int,int> mp;
    int idcnt = 0;
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 8; j++) {
            int x;
            cin >> x;
            if (x != 0) {
                if (!mp.count(x)) mp[x] = idcnt++;
                grid[i][j] = mp[x];
            } else {
                grid[i][j] = -1;
            }
        }
    }
    int n = idcnt;
    vector<int> pre(n, 0);
    for (int c = 0; c < 8; c++) {    // 自底向上
        int last = -1;
        for (int r = 11; r >= 0; r--) {
            int x = grid[r][c];
            if (x == -1) continue;
            if (last != -1 && last != x) {
                pre[x] |= (1 << last);
            }
            last = x;
        }
    }

    int q;
    cin >> q;
    while (q--) {
        int k;
        cin >> k;
        vector<int> b(k);
        for (int i = 0; i < k; i++) {
            int x;
            cin >> x;
            b[i] = mp[x];
        }
        vector<int> pos(n, -1);
        for (int i = 0; i < k; i++) pos[b[i]] = i;
        bool bad = false;
        for (int u = 0; u < n; u++)
            for (int v = 0; v < n; v++)
                if (pre[v] & (1 << u))
                    if (pos[u] != -1 && pos[v] != -1)
                        if (pos[u] > pos[v]) bad = true;
        if (bad) {
            cout << 0 << "\n";
            continue;
        }
        vector<int> inB(n, 0);
        for (int x : b) inB[x] = 1;
        vector<int> newId(n, -1);
        int S = 0;
        int cur = 1;
        for (int i = 0; i < n; i++) {
            if (!inB[i]) newId[i] = cur++;
        }
        int n2 = cur;
        vector<int> pre2(n2, 0);
        for (int v = 0; v < n; v++) {
            for (int u = 0; u < n; u++) {
                if (pre[v] & (1 << u)) {
                    int U = inB[u] ? S : newId[u];
                    int V = inB[v] ? S : newId[v];
                    if (U != V) pre2[V] |= (1 << U);
                }
            }
        }

        LL ans = dp(n2, pre2);
        cout << ans << "\n";
    }

    return 0;
}
