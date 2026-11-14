#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int H, W;
    cin >> H >> W;
    vector<string> grid(H);
    for (auto &row : grid) cin >> row;

    int r = 0, c = 0;
    while (true) {
        if (grid[r][c] == 'v') {
            if (r + 1 == H) break;
            ++r;
        } else {
            if (c + 1 == W) break;
            ++c;
        }
    }
    cout << r + 1 << ' ' << c + 1 << '\n';
    return 0;
}
