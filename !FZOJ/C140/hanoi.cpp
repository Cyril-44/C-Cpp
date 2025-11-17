#include <stdio.h>
#include <string.h>
constexpr int N = 65;
template<typename T>
inline void umin(T &x, const T &y) { x = x < y ? x : y; }
unsigned long long f[N][N]; // 用 i 根柱子 将 j 个盘子 从 1 移动到 n
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int j = 1; j <= n; j++)
        f[3][j] = (1ull << j) - 1ll;
    for (int i = 4; i <= m; i++) {
        f[i][1] = 1;
        for (int j = 2; j <= n; j++) {
            f[i][j] = 1ll << 62;
            for (int k = 1; k < j; k++)
                umin(f[i][j], f[i][k] * 2 + f[i-1][j-k]);
        }
    }
    printf("%llu\n", f[m][n]);
    return 0;
}