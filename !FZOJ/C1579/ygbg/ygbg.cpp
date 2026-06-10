#include <cstdio>
#include <cstring>
#include <algorithm>
#include <numeric>
#include <algorithm>
constexpr int N = 105, MOD = (int)1e9+7;
int a[N], pos[N], fpos[N]; // fpos[i] 是保留 1..i 的情况下 i 在排列中是第几位
struct Mint {
    Mint& operator+=(const Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(const Mint o) { if ((val -= o.val) < 0) val += MOD;    return *this; }
    Mint& operator*=(const Mint o) { val = 1ull*val*o.val % MOD;            return *this; }
    friend Mint operator+(Mint x, const Mint y) { return x += y; }
    friend Mint operator-(Mint x, const Mint y) { return x -= y; }
    friend Mint operator*(Mint x, const Mint y) { return x *= y; }
    Mint(int v = 0) : val(v) {}
private: int val;
} f[N][N], g[N][N][N], h[N][N][N], C[N][N], gsum[N][N], hsum[N][N], gsum2[N][N];
/* ~~不~~难发现，题目要求 i,i+1 的相对顺序保持不变。
   而最小的交换个数，就是原排列中每一个数对与现在排列位置的相对顺序发生变化的个数。换句话说，原排列映射到 1~n，新排列的逆序对数量就是最小的交换个数。
   f[i][j] 表示考虑到数字 i，i 在当前排列中排在第 j 个
   g[y][i][j] 表示只考虑数字 [x,y]，钦定 x 的相对位置为 i，y 的相对位置为 j 时序列的方案数
   h[y][i][j] 表示 …… 所有序列中，“x” 与别的元素产生了相对顺序交换的个数
*/
#define For(i, s, t) for (int i = (s); i <= (t); ++i)
#define roF(i, s, t) for (int i = (s); i >= (t); --i)
int main() {
    int n, type;
    scanf("%d%d", &n, &type);
    For(i, 1, n) {
        scanf("%d", &a[i]);
        pos[a[i]] = i;
    }
    For(i, 0, n) {
        C[i][0] = C[i][i] = 1;
        For(j, 1, i-1) C[i][j] = C[i-1][j-1] + C[i-1][j];
    }
    f[1][1] = 1;
    For(i, 1, n-1) For(j, 1, i)
        if (pos[i] < pos[i+1]) // k 放到 j 后面
            For(k, j+1, i+1)
                f[i+1][k] += f[i][j];
        else // k 放到 j 前面
            For(k, 1, j)
                f[i+1][k] += f[i][j];
    Mint ans1 = std::accumulate(f[n]+1, f[n]+n+1, Mint());
    if (type == 1) return printf("%d\n", ans1), 0;
    Mint ans2;
    For(x, 1, n) { // 计算 >x 的数与 x 组成的所有 “逆序对” 数量
        memset(g, 0, sizeof g), memset(h, 0, sizeof h);
        g[x][1][1] = 1;
        For(y, x, n-1) {
            if (pos[y] < pos[y+1]) { // 此时贡献方向 j-->k 满足 j<k
                For(i, 1, y-x+1) { // 维护一个前缀和
                    gsum[i][1] = hsum[i][1] = gsum2[i][1] = 0;
                    For(j, 2, y-x+2) {
                        gsum[i][j] = gsum[i][j-1] + g[y][i][j-1];
                        hsum[i][j] = hsum[i][j-1] + h[y][i][j-1];
                        gsum2[i][j] = gsum2[i][j-1] + (pos[y+1] < pos[x]) * g[y][i][j-1];
                    }
                }
            } else { // 此时贡献方向 j-->k 满足 j>k
                For(i, 1, y-x+1) { // 维护一个后缀和
                    gsum[i][y-x+2] = hsum[i][y-x+2] = gsum2[i][y-x+2] = 0;
                    roF(j, y-x+1, 1) {
                        gsum[i][j] = gsum[i][j+1] + g[y][i][j];
                        hsum[i][j] = hsum[i][j+1] + h[y][i][j];
                        gsum2[i][j] = gsum2[i][j+1] + (pos[y+1] < pos[x]) * g[y][i][j];
                    }
                }
            }
            For(i, 1, y-x+1) For(k, 1, y-x+2)
                if (k <= i) {
                    g[y+1][i+1][k] += gsum[i][k];
                    h[y+1][i+1][k] += hsum[i][k] + gsum[i][k] - gsum2[i][k];
                } else {
                    g[y+1][i][k] += gsum[i][k];
                    h[y+1][i][k] += hsum[i][k] + gsum2[i][k];
                }
            /* For(i, 1, y-x+1) For(j, 1, y-x+1) {
                int dk, uk;
                if (pos[y] < pos[y+1]) // k 放到 j 后面
                    dk = j+1, uk = y-x+2;
                else dk = 1,  uk = j;
                For (k, dk, uk)
                    if (k <= i) { // 放在 x 前面
                        g[y+1][i+1][k] += g[y][i][j];
                        h[y+1][i+1][k] += h[y][i][j];
                        if (pos[y+1] > pos[x]) h[y+1][i+1][k] += g[y][i][j]; // 实际上不在 x 前面
                    } else { // 放在 x 后面
                        g[y+1][i][k] += g[y][i][j];
                        h[y+1][i][k] += h[y][i][j];
                        if (pos[y+1] < pos[x]) h[y+1][i][k] += g[y][i][j]; // 实际上不在 x 后面
                    }
            } */
        }
        For(i, 1, n-x+1) { // 枚举 x 在新序列的相对位置
            Mint sum = std::accumulate(h[n][i]+1, h[n][i]+n+1, Mint());
            For(j, 1, x) // 枚举 x 在旧序列的相对位置
                ans2 += sum * f[x][j] * C[i-1 + j-1][i-1] * C[n-x+1-i + x-j][n-x+1-i];
        }

    }
    printf("%d %d\n", ans1, ans2);
    return 0;
}