#include <bits/stdc++.h>
constexpr int N = 25, MOD = 998244353;
std::vector<int> g[N], lvchain;
int fa[N];
inline int find(int u) { return fa[u] == u ? u : (fa[u] = find(fa[u])); }
inline int qpow(int b, int n) {
    int res = 1;
    for (; n; n >>= 1, b = 1ull*b*b%MOD)
        if (n & 1) res = 1ull*res*b%MOD;
    return res;
}
void dfs(int u) {
    if (g[u].empty()) lvchain.push_back(u);
    else for (int v : g[u]) dfs(v);
}
int main() {
    int T, n;
    scanf("%d", &T);
    while (T--) {
        std::vector<std::pair<int,int>> edgs;
        scanf("%d", &n);
        lvchain.clear(), lvchain.reserve(n);
        int rt = 0;
        for (int i = 1; i <= n; i++) {
            int c; scanf("%d", &c);
            if (c && !rt) rt = i;
            g[i].resize(c);
            for (int &v : g[i]) scanf("%d", &v), edgs.emplace_back(i, v);
        }
        dfs(rt);
        for (int i = 1; i < (int)lvchain.size(); i++)
            edgs.emplace_back(lvchain[i-1], lvchain[i]);
        int m = edgs.size();
        int ans = 0;
        for (int s = 0; s < (1 << m); s++) {
            std::iota(fa+1, fa+1+n, 1);
            for (int i = 0; i < m; i++)
                if (s >> i & 1) fa[find(edgs[i].first)] = find(edgs[i].second);
            for (int i = 1; i <= n; i++)
                if (i == fa[i]) ++ans;
        }
        printf("%d\n", int(1ull * ans * qpow(1<<m, MOD-2) % MOD));
    }
    return 0;
}