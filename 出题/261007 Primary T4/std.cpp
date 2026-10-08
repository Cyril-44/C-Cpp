#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<string> board(2);
    cin >> board[0] >> board[1];

    vector<vector<int>> dist(2, vector<int>(n, -1));
    queue<pair<int, int>> q;
    int whiteCount = 0;
    for (int r = 0; r < 2; ++r) {
        for (int c = 0; c < n; ++c) {
            if (board[r][c] == '.') ++whiteCount;
            if (c == 0 && board[r][c] == '.') {
                dist[r][c] = 1; // 距离表示经过的格子数
                q.push({r, c});
            }
        }
    }

    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};
    int shortest = -1;
    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();
        if (c == n - 1) {
            shortest = dist[r][c];
            break;
        }
        for (int k = 0; k < 4; ++k) {
            int nr = r + dr[k], nc = c + dc[k];
            if (nr < 0 || nr >= 2 || nc < 0 || nc >= n) continue;
            if (board[nr][nc] == '#' || dist[nr][nc] != -1) continue;
            dist[nr][nc] = dist[r][c] + 1;
            q.push({nr, nc});
        }
    }

    cout << whiteCount - shortest << '\n';
    return 0;
}
