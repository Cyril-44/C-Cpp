#include <bits/stdc++.h>
using namespace std;

string S, T;
int n, K;
vector<vector<int>> dp; // -1: unknown, 0: false, 1: true

bool dfs(int used, int remainK) {
    if (remainK == 0) {
        return used == n;
    }
    int &memo = dp[used][remainK];
    if (memo != -1) return memo;

    int remainLen = n - used;
    // 剩余长度不够分成 remainK 段
    if (remainLen < remainK) return memo = 0;

    // 下一段长度 len
    // 至少 1，最多 remainLen - (remainK - 1)
    int maxLen = remainLen - (remainK - 1);
    for (int len = 1; len <= maxLen; ++len) {
        int sL = used;
        int sR = used + len - 1;
        int tR = n - used - 1;
        int tL = tR - len + 1;
        // 检查 S[sL..sR] == T[tL..tR]
        bool ok = true;
        for (int i = 0; i < len; ++i) {
            if (S[sL + i] != T[tL + i]) {
                ok = false;
                break;
            }
        }
        if (!ok) continue;
        if (dfs(used + len, remainK - 1)) {
            return memo = 1;
        }
    }
    return memo = 0;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int M;
    if (!(cin >> M)) return 0;
    while (M--) {
        cin >> n >> K;
        cin >> S >> T

        if ((int)S.size() != n || (int)T.size() != n) {
            cout << "NO\n";
            continue;
        }

        // 快速剪枝：字符多重集不同，必然不行
        string s1 = S, s2 = T;
        sort(s1.begin(), s1.end());
        sort(s2.begin(), s2.end());
        if (s1 != s2) {
            cout << "NO\n";
            continue;
        }

        if (K == 1) {
            cout << (S == T ? "YES\n" : "NO\n");
            continue;
        }
        if (K > n) {
            cout << "NO\n";
            continue;
        }

        dp.assign(n + 1, vector<int>(K + 1, -1));
        bool ans = dfs(0, K);
        cout << (ans ? "YES\n" : "NO\n");
    }
    return 0;
}
