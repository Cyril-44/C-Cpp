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
    int workingCount = 0;
    for (const string &row : board)
        for (char cell : row)
            if (cell == '.') ++workingCount;

    dist[0][0] = 1;
    q.push({0, 0});
    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();
        for (int k = 0; k < 4; ++k) {
            int nr = r + dr[k], nc = c + dc[k];
            if (nr < 0 || nr >= 2 || nc < 0 || nc >= n) continue;
            if (board[nr][nc] == '#' || dist[nr][nc] != -1) continue;
            dist[nr][nc] = dist[r][c] + 1;
            q.push({nr, nc});
        }
    }

    cout << workingCount - dist[1][n - 1] << '\n';
    return 0;
}
