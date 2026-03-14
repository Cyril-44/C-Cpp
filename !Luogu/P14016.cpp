#include <cstdio>
#include <algorithm>
#include <cstring>
#include <vector>
constexpr int N = 5005, MOD = 998244353;
using ULL = unsigned long long;
struct ModInt {
    inline void fixPos() { if (val >= MOD) val -= MOD; }
    inline void fixNeg() { if (val < 0) val += MOD; }
    inline ModInt(int x = 0) : val(x) {}
    inline explicit operator int() const { return val; }
    inline ModInt operator+=(ModInt x) { val += x.val; fixPos(); return *this; }
    inline ModInt operator-=(ModInt x) { val -= x.val; fixNeg(); return *this; }
    inline ModInt operator*=(ModInt x) { val = (ULL)val * x.val % MOD; return *this; }
    inline ModInt operator+(ModInt x) const { return ModInt(*this) += x; }
    inline ModInt operator-(ModInt x) const { return ModInt(*this) -= x; }
    inline ModInt operator*(ModInt x) const { return ModInt(*this) *= x; }
    inline ModInt& operator++() { if (++val == MOD) val = 0; return *this; }
    inline ModInt& operator--() { if (val-- == 0) val = MOD - 1; return *this; }
    private: int val;
};
ModInt fact[N], ifact[N], inv[N], f[N][N], g[N], ig[N];
int fa[N], size[N];
std::vector<int> edgs[N];
inline int qpow(int b, int n) {
    int res = 1;
    while (n) {
        if (n & 1) res = (ULL)res * b % MOD;
        b = (ULL)b * b % MOD;
        n >>= 1;
    }
    return res;
}
inline ModInt binom(int n, int m) {
    if (n < 0 || m < 0 || n < m) return 0;
    return fact[n] * ifact[m] * ifact[n - m];
}
int n = 5000;
/*
f[u][j] 表示除去点 u 的子树之外的所有点构成拓扑序之后，i 排在第 j 个的方案数。
可以理解为是一个排列集合，其中的每一个排列对应一种拓扑序列，在这个序列中，节点 u 排在第 j 位。
因为 u 的子树节点在拓扑序列中一定表示在 u 的后面，所以 j 是固定的。
考虑转移，f[u][j] --> f[v][k]，现在考虑这样处理：
选中所有 u(不包含 u) 子树内 且 不属于 v 子树的点。
这些点应该会放在原来的排列中节点 i 的后面，而节点 i 后面已经有 (n - sz[i] + 1 - j) 个元素。
那如果这些选中的点已经有一个相对的拓扑序，那这就意味着我们可以直接将这个拓扑序插入到 i 后面的这些元素中间去（即不改变相对顺序组合两个序列）
而有定理：对于一个树，其拓扑序列数量为 \dfrac{n!}{\prod_{i=1}^n sz_i}
那转移就是 f[v][k] = f[u][j] * \dfrac{(sz[u] - sz[v] - 1)!}{\prod_{i \in S_u - S_v} sz_i} * \dbinom{(n - sz[u] + 1 - j) + (sz[u] - sz[v] - 1)}{sz[u] - sz[v] - 1}
注意到转移与 k 无关，前缀和优化，区间 [j, j + sz[u] - sz[v]]
于是这里的 v 随便插在里面一个地方就可以了
*/
void dfs(int u) {
    for (int v : edgs[u]) {
        int sel = size[u] - size[v] - 1;
        ModInt base = fact[sel] * ig[u] * g[v];
        for (int i = 1; i <= n - size[u] + 1; i++) {
            ModInt val = f[u][i] * base * binom(n - i - size[v], sel);
            if (!int(val)) continue;
            f[v][i+1] += val;
            f[v][n - size[v] + 2] -= val;
            // fprintf(stderr, "(%d,%d) --> (%d, %d~%d) + %d\n", u, i, v, i+1, i + sel + 1, int(val));
        }
        for (int i = 1; i <= n; i++) f[v][i] += f[v][i-1]; // 差分意义
        dfs(v);
    }
}
int main() {
    fact[0] = 1;
    for (int i = 1; i <= n; i++) fact[i] = fact[i-1] * ModInt(i);
    ifact[n] = qpow(int(fact[n]), MOD - 2);
    for (int i = n-1; i >= 0; i--) ifact[i] = ifact[i+1] * ModInt(i+1);
    for (int i = 1; i <= n; i++) inv[i] = ifact[i] * fact[i-1];
    scanf("%d", &n);
    for (int i = 2; i <= n; i++) {
        scanf("%d", &fa[i]);
        edgs[fa[i]].push_back(i);
    }
    for (int i = 1; i <= n; i++) g[i] = ig[i] = 1;
    for (int i = n; i >= 1; i--) size[fa[i]] += ++size[i], g[fa[i]] *= g[i] *= size[i], ig[fa[i]] *= ig[i] * inv[size[i]];
    // for (int i = 1; i <= n; i++) fprintf(stderr, "%d %d %d\n", size[i], int(g[i]), int(ig[i]));
    f[1][1] = 1; dfs(1);
    // for (int i = 1; i <= n; i++)
    //     for (int j = 1; j <= n; j++)
    //         fprintf(stderr, "%d%c", int(f[i][j]), j==n?'\n':' ');
    for (int i = 1; i <= n; i++)
        printf("%d ", int(f[i][i] * fact[size[i] - 1] * ig[i] * binom(n - i, size[i] - 1)));
    putchar('\n');
    return 0;
}