#include <bits/stdc++.h>
constexpr int N = 25;
std::vector<int> g[N];
int edges;
int n, T;
inline void output() {
    if (++T % 10000 == 0) std::cerr << "Finished " << T << std::endl;
    std::cout << n << ' ' << edges << '\n';
    for (int i = 1; i <= n; i++)
        for (int v : g[i])
            if (v > i)
                std::cout << i << ' ' << v << '\n';
}
void dfs(int u) {
    if (u == n + 1) return output();
    if (g[u].size() < 3) {
        for (int i = u+1; i <= n; i++) if (g[i].size() < 3) {
            g[u].push_back(i), g[i].push_back(u), ++edges;
            if (g[u].size() < 2)
            for (int j = i; j <= n; j++) if (g[j].size() < 3) {
                g[u].push_back(j), g[j].push_back(u), ++edges;
                if (g[u].size() < 1)
                for (int k = j; k <= n; k++) if (g[k].size() < 3) {
                    g[u].push_back(k), g[k].push_back(u), ++edges;
                    dfs(u+1);
                    g[u].pop_back(), g[k].pop_back(), --edges;
                }
                dfs(u+1);
                g[u].pop_back(), g[j].pop_back(), --edges;
            }
            dfs(u+1);
            g[u].pop_back(), g[i].pop_back(), --edges;
        }
    }
    dfs(u+1);
}
int main(int argc, char **argv) {
    std::ios::sync_with_stdio(false);
    n = atoi(argv[1]);
    dfs(1);
    std::cerr << "Total: " << T << std::endl;
    return 0;
}