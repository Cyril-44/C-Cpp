#include <cstdio>
#include <bitset>
#include <vector>
#include <cstring>
constexpr int N = 50005, M = 100005, B = 2048;
std::bitset<B> f[N], // f 表示当前节点有多少个限制条件能到达。即，f[u] 表示在限制条件 i(0<=i<64) 下，点 u 可否被到达
               s[M]; // s 表示当前边可被用的限制条件集合
std::vector<std::pair<int,int>> g[N];
int terminal[B];
inline void partsolve(int n, int m, int q) {
    for (int i = 1; i <= m; i++) s[i] ^= s[i-1];
    for (int u = 1; u <= n; u++)
        for (const auto &[v, id] : g[u])
            f[v] |= f[u] & s[id];
    for (int i = 0; i ^ q; i++)
        puts(f[terminal[i]][i] ? "YES" : "NO");
    for (int i = 1; i <= m; i++) s[i].reset();
    for (int i = 1; i <= n; i++) f[i].reset();
}
inline void solveSingle() {
    int n, m, q;
    scanf("%d%d%d", &n, &m, &q);
    for (int i = 1; i <= n; i++) g[i].clear();
    for (int u, v, id=1; id<=m; ++id) {
        scanf("%d%d", &u, &v);
        g[u].emplace_back(v, id);
    }
    int idx = 0;
    for (int u, v, l, r; q--; (++idx == B) && (partsolve(n, m, idx), idx = 0)) {
        scanf("%d%d%d%d", &u, &terminal[idx], &l, &r);
        f[u].set(idx);
        s[l].set(idx), s[r+1].set(idx); // 维护异或和
    }
    if (idx) partsolve(n, m, idx);
}
int main() {
    int T; scanf("%d", &T); while (T--) solveSingle();
}
