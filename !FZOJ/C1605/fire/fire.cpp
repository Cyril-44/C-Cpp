#include <cstdio>
#include <random>
#include <vector>
#include <unordered_map>
#include <algorithm>
#ifdef CLANGD
constexpr int N = 4;
#else
constexpr int N = 1000004;
#endif
std::mt19937_64 rng(std::random_device{}());
std::vector<int> g[N];
uint64_t s[N], a[N];
std::unordered_map<uint64_t, int> mp;
void dfs(int u, int fa) {
    for (int v : g[u]) if (v != fa) {
        dfs(v, u);
        s[u] ^= s[v] ^= a[v];
        mp[s[v]]++;
    }
}
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int u, v, i=1; i<n; i++) {
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    for (int u, v; m--; ) {
        scanf("%d%d", &u, &v);
        uint64_t cur = rng();
        a[u] ^= cur, a[v] ^= cur;
    }
    dfs(1, 0);
    int ans = 0;
    for (const auto &[h, v] : mp)
        ans = std::max(ans, v);
    printf("%d\n", n - 1 - ans);
    return 0;
}