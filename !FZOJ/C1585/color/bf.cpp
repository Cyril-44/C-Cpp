#include <bits/stdc++.h>
constexpr int N = 25;
std::vector<std::pair<int,int>> g[N];
int n;
inline bool check(int col) {
    for (int i = 1; i <= n; i++)
        for (const auto &[j, id1] : g[i])
            for (const auto &[k, id2] : g[j])
                if (id1 != id2)
                    if ((col >> i & 1) == (col >> j & 1) && (col >> i & 1) == (col >> k & 1))
                        return false;
    return true;
}
int main() {
    std::cin.tie(nullptr) -> sync_with_stdio(false);
    int T, m;
    std::cin >> T;
    int low = 1e9;
    while (T--) {
        std::cin >> n >> m;
        for (int i = 1; i <= n; i++) g[i].clear();
        int tot = 0;
        while (m--) {
            int u, v; std::cin >> u >> v;
            ++tot;
            g[u].emplace_back(v, tot), g[v].emplace_back(u, tot);
        }
        int found = 0;
        for (int s = 0; s < (1<<n); s++)
            if (check(s<<1)) {
                for (int i = 0; i < n; i++)
                    if (s>>i&1) std::cout << "1 ";
                    else std::cout << "0 ";
                std::cout << '\n';
                ++found;
                // break;
            }
        low = std::min(low, found);
        if (!found) std::cout << "GG\n";
    }
    std::cerr << "Low: " << low << std::endl;
    return 0;
}