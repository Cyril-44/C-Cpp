#include <cstdio>
#include <cstring>
#include <immintrin.h>
#pragma GCC target("avx", "avx2")
constexpr int N = (2005 | 7) + 1, B = 45, MOD = 998244353;
alignas(32) int f[2][B + 1][N], g[N][N];
inline void add(int &x, int y) {
    if ((x += y) >= MOD) x -= MOD;
}
/*
f[i][j][k] 表示有 i 个段，产生贡献有 j 个段，总高度为 k
    1. 往最后面放一个长度为 B 的贡献段 f[i-1][j-1][k-B]
    2. 往最后面放一个长度为 B 的非贡献段 f[i-1][j][k]
    3. 所有贡献段同时+1 f[i][j][k-j]
g(k)[i][j] 表示当前最大的是 k（更加准确的，今后不会再有比 k 大的数），有 i 个段，总高度为 j
    1. 当前不加进去 g[k+1][i][j]
    2. 当前加进去，不贡献 g[k+1][i-1][j]
    3. 当前加进去，贡献 g[k+1][i-1][j-k]
*/
int main() {
    freopen("ladder.in", "r", stdin);
    freopen("ladder.out", "w", stdout);
    const __m256i MODvec = _mm256_set1_epi32(MOD), MOD_1vec = _mm256_set1_epi32(MOD - 1);
#define Mset(x, y) *(__m256i *)&(x) = y
#define Madd(x, y) _mm256_add_epi32(*(__m256i *)(&(x)), _mm256_loadu_si256((__m256i *)(&(y))))
#define Mmod(x)                                                                                                        \
    do {                                                                                                               \
        __m256i vx = *(__m256i *)&(x);                                                                                 \
        vx = _mm256_blendv_epi8(vx, _mm256_sub_epi32(vx, MODvec), _mm256_cmpgt_epi32(vx, MOD_1vec));                   \
        *(__m256i *)(&(x)) = vx;                                                                                       \
    } while (0)
#define AddVec(x, y, start, shift)                                                                                     \
    do {                                                                                                               \
        int startK = (start | 7) + 1;                                                                                  \
        if (startK >= endK || 1 <= shift && shift <= 7)                                                                \
            for (int k = start; k <= n; k++) add(x[k], y[k - shift]);                                                  \
        else {                                                                                                         \
            for (int k = start; k ^ startK; k++) add(x[k], y[k - shift]);                                              \
            for (int k = startK; k ^ endK; k += 8) {                                                                   \
                Mset(x[k], Madd(x[k], y[k - shift]));                                                                  \
                Mmod(x[k]);                                                                                            \
            }                                                                                                          \
            for (int k = endK; k <= n; k++) add(x[k], y[k - shift]);                                                   \
        }                                                                                                              \
        Mmod(x);                                                                                                       \
    } while (0)
    f[0][0][0] = 1;
    const int n = 2000, endK = n & ~7;
    for (int i = 1; i <= n; i++) {
        memcpy(f[i & 1], f[i & 1 ^ 1], sizeof(f) / 2);
        if (i == 1) f[1][0][0] = 0; // 错误的转移。
        for (int j = 1; j <= B; j++) {
            AddVec(f[i & 1][j], f[i & 1 ^ 1][j - 1], B, B);
            AddVec(f[i & 1][j], f[i & 1][j], j, j);
            // for (int k = j; k <= n; k++) {
            //     if (k >= B) add(f[i&1][j][k], f[i&1^1][j-1][k-B]);
            //     if (k >= j) add(f[i&1][j][k], f[i&1][j][k-j]);
            // }
        }
        for (int j = 1; j <= B; j++) {
            AddVec(g[i], f[i & 1][j], 1, 0);
            // for (int k = 1; k <= n; k++) add(g[i][k], f[i & 1][j][k]);
        }
    }
    g[0][0] = 1;
    for (int j = B - 1; j >= 1; j--)
        for (int i = 1; i <= n; i++) {
            AddVec(g[i], g[i - 1], 1, 0);
            AddVec(g[i], g[i - 1], j, j);
            // for (int k = 1; k <= n; k++) {
            //     add(g[i][k], g[i - 1][k]);
            //     if (k >= j) add(g[i][k], g[i - 1][k - j]);
            // }
        }
    int T;
    scanf("%d", &T);
    for (int n, m; T--;) {
        scanf("%d%d", &n, &m);
        printf("%d\n", g[n][m]);
    }
    return 0;
}