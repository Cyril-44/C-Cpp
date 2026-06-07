#include <cassert>
#include <cstdio>
#include <vector>
constexpr int N = 200005, M = 105, MOD = (int)1e9+7;
std::vector<int> g[N];
int nxt[N], a[M], dep[N];
int b[M]; // b[i]: 对于 a[i] 来说最少要剥几次叶子才能让 a[i] 被成功切割
int dfs(int u, int fa = 0) {
    int mxdep = dep[u];
    for (int v : g[u]) if (v != fa) {
        dep[v] = dep[u] + 1;
        int mxvdep = dfs(v, u);
        if (mxvdep > mxdep) mxdep = mxvdep, nxt[u] = v;
    }
    return mxdep;
}
struct ModInt {
    ModInt& operator+=(const ModInt x) { if ((val += x.val) >= MOD) val -= MOD; return *this; }
    ModInt& operator-=(const ModInt x) { if ((val -= x.val) < 0) val += MOD; return *this; }
    ModInt& operator*=(const ModInt x) { val = 1ull*val*x.val%MOD; return *this; }
    friend ModInt operator+(ModInt x, const ModInt y) { return x += y; }
    friend ModInt operator-(ModInt x, const ModInt y) { return x -= y; }
    friend ModInt operator*(ModInt x, const ModInt y) { return x *= y; }
    ModInt(int v=0) : val(v) {}
    template<class T> explicit operator T() const { return static_cast<T>(val); }
private: int val;
} f[M][M][M], C[M][M];
// f[t][i][j] 表示钦定 i~j 被剥了 t 次叶子，最终删空的方案数
// 初始化就是 f[t][i][i] = b[i]<=t，即初始可不可以删除 i
// 转移就是枚举 mid 然后操作 mid，计算方案数
// 最后 t=0 就是 DP 终止
int main() {
    int n;
    scanf("%d", &n);
    for (int u, v, i = 1; i < n; i++) {
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    int m = dfs(1) + 1;
    for (int i = 0; i <= m; i++) {
        C[i][0] = C[i][i] = 1;
        for (int j = 1; j < i; j++)
            C[i][j] = C[i-1][j-1] + C[i-1][j];
    }
    a[1] = 1;
    for (int i = 1; i < m; i++) {
        a[i+1] = nxt[a[i]], b[i+1] = std::max(b[i], i);
        for (int v : g[a[i]]) if (v != a[i-1] && v != a[i+1]) b[i+1] = std::max(b[i+1], dfs(v, a[i]) + 1);
    }
    for (int i = 1; i <= m; i++) b[i] -= i-1;
    // for (int i = 1; i <= m; i++) printf("%d\n", b[i]);
    for (int t = 0; t <= m; t++)
        for (int i = 1; i <= m; i++)
            f[t][i][i] = b[i] <= t;
    for (int t = m; t >= 0; t--) {
        for (int l = 1; l <= m; l++)
            for (int i = 1, j = l; j <= m; i++, j++) {
                if (b[i] > t) continue; // i 本身不可操作，那不可能删干净
                f[t][i][j] += f[t+1][i][j-1]; // 如果就操作 i
                for (int k = i+1; k < j; k++) if (b[k] <= t) // 枚举可操作的 mid
                    f[t][i][j] += f[t][i][k-1] * f[t+1][k][j-1] * C[j-i][k-i]; // [i,k)是剥t次，[k,j)就是剥t+1次。两个操作序列相对顺序不变即可。
                if (b[j] <= t) f[t][i][j] += f[t][i][j-1]; // 如果就操作 j
            }
    }
    printf("%d %d\n", m, f[0][1][m]);
    return 0;
}
