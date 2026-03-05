#include <bits/stdc++.h>
using namespace std;
// 设f[i][j][k]表示i个数，和为j，有k个数产生贡献
// 适用于>sqrt(n)的数
// 在<=sqrt(n)的数中选t个数，每个数可贡献可不贡献，和为p的方案数
// 枚举now，g[i][j]=g[i][j]+g[i-1][j]+g[i-1][j-now]
// 这样直接将g[i][j]作为f[i][j][0]的初值
// 如果>sqrt(n)的数有的被选了，那么g中考虑了贡献和不贡献的数之间的顺序显然是对的
// 再重新算只有<=sqrt(n)的数被选的情况
// 枚举最大的数mx,g[n-1][m-mx]
// 我服了我愿称之为分拆数天花板qwq
const int N = 2005, M = 1e5 + 10, mod = 998244353;
int T, n = 2000, m, g[N][N], f[N][N][45], s[N][N];
void add(int &x, int y) {
    x += y;
    if (x >= mod) x -= mod;
}
int main() {
    freopen("ladder.in", "r", stdin);
    freopen("ladder.out", "w", stdout);
    // 但是n不产生贡献能穿插进行吗？
    // 3,3,3
    // 可以是其中任意一个位置产生贡献，算了三次
    // 1,2
    // 1,1,1
    g[0][0] = 1;
    for (int now = 1; now <= 45; now++) {
        for (int i = 1; i <= n; i++)
            for (int j = 0; j <= n; j++) {
                add(g[i][j], g[i - 1][j]);                     // 选了不贡献
                if (j >= now) add(g[i][j], g[i - 1][j - now]); // now产生贡献
            }
        for (int i = 1; i <= n; i++)
            for (int j = now; j <= n; j++) add(s[i][j], g[i - 1][j - now]);
    }
    for (int i = 0; i <= n; i++)
        for (int j = 0; j <= n; j++) f[i][j][0] = g[i][j];
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            for (int k = 1; k <= 44; k++) {
                if (j >= k) add(f[i][j][k], f[i][j - k][k]);
                if (j >= 46) add(f[i][j][k], f[i - 1][j - 46][k - 1]);
                add(f[i][j][k], f[i - 1][j][k]);
            }
    scanf("%d", &T);
    for (int i = 1; i <= T; i++) {
        scanf("%d%d", &n, &m);
        int res = s[n][m];
        for (int j = 1; j <= 44; j++) add(res, f[n][m][j]);
        printf("%d\n", res);
    }
}