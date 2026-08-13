#include <bits/stdc++.h>
constexpr int N = 105;
int dfs(int x, int y) {
    std::pair<int,int> hist[N];
    int now = 0;
    std::set<std::pair<int,int>> vis;
    vis.emplace(x, y);
    while (x != y) {
        std::tie(x, y) = std::make_tuple(x + y - std::abs(x - y), std::abs(x - y));
        if (!vis.emplace(x, y).second) return 0;
        hist[++now] = {x, y};
    }
    return now + 1;
}
int main() {
    for (int i = 1; i <= 50; i++)
        for (int j = 1; j <= 50; j++)
            printf("%2d%c", dfs(i, j), ",\n"[j==50]);
    while (0) {
        int x, y;
        scanf("%d%d", &x, &y);
        std::cout << (dfs(x, y) == std::gcd(std::abs(x - y), x + y) - 1);
        printf(" %d\n", dfs(x, y));
    }
    return 0;
}