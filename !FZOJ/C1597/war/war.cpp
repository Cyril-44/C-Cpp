#include <cstdio>
#include <cstring>
#define For(i, s, t) for(int i = (s); i <= (t); i++)
#define roF(i, s, t) for(int i = (s); i >= (t); i--)
constexpr int N = 304, MOD = 998244353;
int fa[N], a[N];
struct Mint {
    Mint& operator+=(Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint a, Mint b) { return a += b; }
    friend Mint operator-(Mint a, Mint b) { return a -= b; }
    friend Mint operator*(Mint a, Mint b) { return a *= b; }
    Mint(int o = 0) : val(o) {}
private: int val;
} f[N][N], g[N][N], com[N], inv[N];
/* f: 点 u 有 i 个未使用的断点
 * g: 上传时分裂，i --> j 个断点的转移系数
 */
Mint qpow(Mint b, int n) {
    Mint res = 1;
    while (n) {
        if (n & 1) res *= b;
        b *= b, n >>= 1;
    }
}
int main() {
    inv[1] = 1; For(i, 2, 300) inv[i] = (MOD - MOD / i) * inv[MOD % i];
    int tid, T, n, K;
    scanf("%d%d", &tid, &T);
    while (T--) {
        scanf("%d%d", &n, &K);
        For(i, 2, n) scanf("%d", &fa[i]);
        For(i, 1, n) scanf("%d", &a[i]);
        memset(g, 0, sizeof g);
        com[0] = 1;
        For(i, 1, n) com[i] = com[i-1] * (K-i+1) * inv[i];
        g[0][0] = 1;
        For(i, 1, n) For(j, i, n) For(k, i-1, j-1)
            g[i][j] += g[i-1][k] * com[j-k];
        For(u, 1, n) {
            f[u][0] = 1;
            For(i, 1, n) f[u][i] = f[u][i-1] * (a[u]-i+1) * inv[i];
        }
        roF(u, n, 2) {
            For(i, 0, n-1) f[u][i] += f[u][i+1];
            roF(i, n, 0) {
                f[u][i] *= g[i][i];
                roF(j, i-1, 0) f[u][i] += f[u][j] * g[j][i];
            }
            Mint tmp[N]{};
            For(i, 0, n) For(j, 0, n-i)
                tmp[i + j] += f[fa[u]][i] * f[u][j];
            memcpy(f[fa[u]], tmp, sizeof tmp);
        }
        printf("%d\n", f[1][0]);
    }
    return 0;
}