#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <tuple>
#include <random>
constexpr int N = 200005;
std::vector<std::pair<int, int>> g[N];
bool col[N];
int n;
int main() {
    std::mt19937 rng(std::random_device{}());
    auto select = [&](int x, int y, int z) {
        switch (std::uniform_int_distribution<int>(0,29)(rng)) {
            case 0: return x;
            case 29: return z;
            default: return y;
        }
    };
    int T, m;
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d", &n, &m);
        for (int i = 1; i <= n; i++) g[i].clear();
        memset(col, 0, n+1);
        int tot = 0;
        while (m--) {
            int u, v; scanf("%d%d", &u, &v);
            ++tot;
            g[u].emplace_back(v, tot), g[v].emplace_back(u, tot);
        }

        std::vector<std::tuple<int,int,int>> all;
        for (int i = 1; i <= n; i++)
            for (const auto &[j, id1] : g[i])
                for (const auto &[k, id2] : g[j])
                    if (id1 != id2)
                        all.emplace_back(i, j, k);
        for (int _ = 1; _ <= 7; _++) {
            for (const auto &[i, j, k] : all)
                if (col[i] == col[j] && col[i] == col[k]) {
                    int x = j;
                    col[x] = !col[x];
                }
        }
        for (int i = 1; i <= n; i++)
            putchar(col[i] + '0'), putchar(' ');
        putchar('\n');
    }
    return 0;
}