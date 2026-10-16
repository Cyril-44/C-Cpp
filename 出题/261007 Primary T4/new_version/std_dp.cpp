#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<string> board(2);
    cin >> board[0] >> board[1];

    int workingCount = 0;
    for (const string &row : board)
        for (char cell : row)
            if (cell == '.') ++workingCount;

    const int INF = 1e9;
    // dp[r]：到达当前列第 r 行工作中继器的最少中继器数。
    int dp[2] = {INF, INF};
    if (board[0][0] == '.') dp[0] = 1;
    if (board[0][0] == '.' && board[1][0] == '.') dp[1] = 2;

    for (int c = 1; c < n; ++c) {
        int next[2] = {INF, INF};
        for (int r = 0; r < 2; ++r)
            if (board[r][c] == '.') next[r] = dp[r] + 1;

        if (board[0][c] == '.' && board[1][c] == '.') {
            next[0] = min(next[0], dp[1] + 2);
            next[1] = min(next[1], dp[0] + 2);
        }
        dp[0] = next[0];
        dp[1] = next[1];
    }

    cout << workingCount - dp[1] << '\n';
    return 0;
}
