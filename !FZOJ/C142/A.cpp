#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <vector>
constexpr int N = 100005;
std::vector<int> g[N];
int k, sz[N];
using ULL = unsigned long long;
constexpr int MOD = 1e9+7;
inline int qpow(ULL b, int n) {
    ULL res = 1;
    while (n) {
        if (n & 1) res = res * b % MOD;
        b = b * b % MOD;
        n >>= 1;
    }
    return res;
}
std::pair<int, bool> fa[N];
void dfs(int u, int pre) {
    sz[u] = 1;
    for (int v : g[u]) {
        if (v == pre) continue;
        dfs(v, u);
        sz[u] += sz[v];
    }
}
int top[N];
int main() {
    int n;
    scanf("%d%d", &n, &k);
    for (int u, v, w, i = n; --i; ) {
        scanf("%d%d%d", &u, &v, &w);
        if (!w) {
            g[u].emplace_back(v);
            g[v].emplace_back(u);
        }
    }
    int ans = 0;
    for (int i = 1; i <= n; i++)
        if (!sz[i]) {
            dfs(i, 0);
            ans += qpow(sz[i], k);
            if (ans >= MOD) ans -= MOD;
        }
    ans = (qpow(n, k) - ans + MOD) % MOD;
    printf("%d\n", ans);
    return 0;
}
