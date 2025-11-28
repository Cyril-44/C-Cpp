#include <iostream>
#include <vector>
#include <queue>
#include <tuple>

using namespace std;

const int dx[] = {-1, 1, 0, 0};
const int dy[] = {0, 0, -1, 1};

struct Tree {
    int x, y, time;
};

int main() {
    freopen("lake.in", "r", stdin);
    freopen("lake.out", "w", stdout);
    int n, m, q, r, k;
    cin >> n >> m >> q >> r >> k;

    vector<vector<int>> grid(n, vector<int>(m, 0)); // 0: land, 1: lake, 2: tree
    vector<vector<int>> treeTime(n, vector<int>(m, -1)); // Time when tree was planted or grew

    // Read lakes
    for (int i = 0; i < q; ++i) {
        int a1, b1, a2, b2;
        cin >> a1 >> b1 >> a2 >> b2;
        for (int x = a1 - 1; x < a2; ++x) {
            for (int y = b1 - 1; y < b2; ++y) {
                grid[x][y] = 1;
            }
        }
    }

    queue<Tree> qTree;
    // Read tree plantings
    for (int i = 0; i < r; ++i) {
        int t, x, y;
        cin >> t >> x >> y;
        --x; --y;
        grid[x][y] = 2;
        treeTime[x][y] = t;
        qTree.push({x, y, t});
    }

    // Simulate tree growth
    while (!qTree.empty()) {
        auto [x, y, time] = qTree.front();
        qTree.pop();

        for (int d = 0; d < 4; ++d) {
            int nx = x + dx[d], ny = y + dy[d];
            if (nx >= 0 && nx < n && ny >= 0 && ny < m && grid[nx][ny] == 0) {
                bool adjacentToLake = false, adjacentToTree = false;
                for (int d2 = 0; d2 < 4; ++d2) {
                    int nnx = nx + dx[d2], nny = ny + dy[d2];
                    if (nnx >= 0 && nnx < n && nny >= 0 && nny < m) {
                        if (grid[nnx][nny] == 1) adjacentToLake = true;
                        if (grid[nnx][nny] == 2 && treeTime[nnx][nny] == time) adjacentToTree = true;
                    }
                }
                if (adjacentToLake && adjacentToTree) {
                    grid[nx][ny] = 2;
                    treeTime[nx][ny] = time + 1;
                    qTree.push({nx, ny, time + 1});
                }
            }
        }
    }

    // Count final number of trees
    int treeCount = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] == 2) {
                bool allAdjacentLand = true;
                for (int d = 0; d < 4; ++d) {
                    int ni = i + dx[d], nj = j + dy[d];
                    if (ni >= 0 && ni < n && nj >= 0 && nj < m && grid[ni][nj] == 2) {
                        allAdjacentLand = false;
                        break;
                    }
                }
                if (treeTime[i][j] + k <= treeTime[i][j] && allAdjacentLand) {
                    grid[i][j] = 0;
                } else {
                    ++treeCount;
                }
            }
        }
    }

    cout << treeCount << endl;
    return 0;
}
