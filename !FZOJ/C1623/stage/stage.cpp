#include <bits/stdc++.h>
#define For(i, s, t) for (int i = (s); i <= (t); i++)
#define roF(i, s, t) for (int i = (s); i >= (t); i--)
constexpr int N = 2005;
int a[N][N]; int64_t s[N][N], fl1[N][N], fr1[N][N], fl4[N][N], fr4[N][N];
int64_t sum(int ul, int ur, int dl, int dr) { return s[dl][dr] - s[dl][ur-1] - s[ul-1][dr] + s[ul-1][ur-1]; }
int main() {
    std::cin.tie(nullptr)->sync_with_stdio(false);
    int n, m; int64_t ans = -1e18; std::cin >> n >> m;
    For(i, 1, n) For(j, 1, m) std::cin >> a[i][j];
    For(i, 1, n) For(j, 1, m) s[i][j] = s[i-1][j] + s[i][j-1] - s[i-1][j-1] + a[i][j];
    For(i, 1, n) For(j, 1, m) fl1[i][j] = std::max(fl1[i-1][j] + sum(i, 1, i, j) * 3, fl1[i][j-1] + sum(1, j, i, j) * 1);
    For(i, 1, n) roF(j, m, 1) fr1[i][j] = std::max(fr1[i-1][j] + sum(i, j, i, m) * 2, fr1[i][j+1] + sum(1, j, i, j) * 1);
    roF(i, n, 1) For(j, 1, m) fl4[i][j] = std::max(fl4[i+1][j] + sum(i, 1, i, j) * 3, fl4[i][j-1] + sum(i, j, n, j) * 4);
    roF(i, n, 1) roF(j, m, 1) fr4[i][j] = std::max(fr4[i+1][j] + sum(i, j, i, m) * 2, fr4[i][j+1] + sum(i, j, n, j) * 4);
    For(i, 0, n) For(j, 0, m) ans = std::max(ans, fl1[i][j] + fr1[i][j+1] + fl4[i+1][j] + fr4[i+1][j+1]);
    std::cout << ans << '\n';
}