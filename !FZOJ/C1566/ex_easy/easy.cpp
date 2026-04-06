#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#define For(i, s, t) for (int i = (s); i <= (t); ++i)
#define roF(i, s, t) for (int i = (s); i >= (t); --i)
constexpr int N = 305, MOD = (int)1e9 + 7;
struct ModInt {
    inline ModInt(int x = 0) : val(x) {}
    template<typename T> inline explicit operator T() const { return static_cast<T>(val); }
    inline ModInt& operator+=(ModInt o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    inline ModInt& operator-=(ModInt o) { if ((val -= o.val) < 0) val += MOD;    return *this; }
    inline ModInt& operator*=(ModInt o) { val = 1ull * val * o.val % MOD;        return *this; }
    inline ModInt operator+(ModInt o) const { return ModInt(*this) += o; }
    inline ModInt operator-(ModInt o) const { return ModInt(*this) -= o; }
    inline ModInt operator*(ModInt o) const { return ModInt(*this) *= o; }
    private: int val;
} f[N][N][N];
#ifdef ONLINE_JUDGE
#define fprintf(...) ((void)0)
#endif
int n, m, x, y;
int b[N * 2];
std::vector<int> e[N];
void dfs(int u) {
    ModInt g[N][N]{}; // g[i][j] 表示计算 au 的情况下 maxanc=i，不计算 au 的情况下 maxsub=j。
    For (i, 1, m) g[i][1] = 1;
    for (int v : e[u]) {
        dfs(v);
        ModInt sg[N][N], sf[N][N];
        For (i, 1, m)
            For (j, 1, m)
                sg[i][j] = sg[i][j-1] + g[i][j],
                sf[i][j] = sf[i][j-1] + f[v][i][j];
        For (i, 1, m)
            For (j, 1, m)
                g[i][j] = sg[i][j] * f[v][i][j] + g[i][j] * sf[i][j] - g[i][j] * f[v][i][j];
        /* ModInt h[N][N];
        For (i, 1, m)
            For (j1, 1, m)
                For (j2, 1, m)
                    h[i][std::max(j1, j2)] += g[i][j1] * f[v][i][j2];
        memcpy(g, h, sizeof g); */
    }
    if (u != 1) {
        /* For (i, 1, m)
            For (j, 1, m) {
                // au = i ==> 上面的 maxanc 可以为任意值
                For (k, 1, i)
                    f[u][k][std::max(i, j)] += g[i][j] * b[x*i + y*std::max(i, j)];
            } */
        // 先处理 au = i
        ModInt dif[N][N];
        For (i, 1, m)
            For (j, 1, m) {
                ModInt val = g[i][j] * b[x*i + y*std::max(i,j)];
                dif[1][std::max(i, j)] += val;
                dif[i+1][std::max(i, j)] -= val;
            }
        /* For (i, 1, m)
            For (j, 1, m) {
                // au < i ==> 上面的 maxanc 还是 i
                For (au, 1, i-1)
                    f[u][i][std::max(au, j)] += g[i][j] * b[x*i + y*std::max(au, j)];
            } */
        // 再处理 au < i，技巧：t[i][j] 转移系数都是 b[x*i+y*j]
        For (i, 1, m)
            For (j, 1, m)
                if (i-1 > j) // f[i][j+1 ~ i-1] += g[i][j]
                    f[u][i][j+1] += g[i][j], f[u][i][i] -= g[i][j];
        For (i, 1, m)
            For (j, 1, m)
                f[u][i][j] += f[u][i][j-1];
        For (i, 2, m)
            For (j, 1, m)
                f[u][i][j] += g[i][j] * std::min(i-1, j);
        For (i, 1, m)
            For (j, 1, m)
                f[u][i][j] *= b[x*i + y*j];
        // 最后加上 au = i 的贡献
        For (i, 1, m)
            For (j, 1, m) {
                dif[i][j] += dif[i-1][j];
                f[u][i][j] += dif[i][j];
            }
    }
    else {
        For (i, 1, m)
            For (j, 1, m) {
                // au = i
                f[u][i][std::max(i, j)] += g[i][j] * b[x*i + y*std::max(i, j)];
            }
    }
}
int main() {
    scanf("%d%d%d%d", &n, &m, &x, &y);
    for (int i = 1; i <= 2*m; i++) scanf("%d", &b[i]);
    for (int i = 2, p; i <= n; i++) scanf("%d", &p), e[p].push_back(i);
    dfs(1);
    ModInt ans;
    For (i, 1, m)
        For (j, 1, m)
            ans += f[1][i][j];
    printf("%d\n", int(ans));
    return 0;
}