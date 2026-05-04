#include <cstdio>
#include <algorithm>
#include <vector>
#include <map>
#include <cstdint>
#include <cstring>
constexpr int N = 405;
char s[N];
int n, mask;
struct BF {
    long double f[1 << 15];
    bool vis[1 << 15];
    long double dfs(uint16_t st) {
        if (vis[st]) return f[st];
        int maxfront = __builtin_ctz(st^mask);
        uint16_t curst = st;
        for (int i = 1; i < n; i++) {
            curst = curst >> 1 | (curst & 1) << (n-1);
            int cnt = __builtin_ctz(curst^mask);
            if (cnt > maxfront) maxfront = cnt, st = curst;
        }
        for (int i = 0, cnt; i < n; i++) {
            for (cnt = 0; i + cnt < n && (st >> i+cnt & 1); ++cnt);
            i += cnt;
            f[st] += (cnt * (cnt + 1) >> 1) / (long double)n;
            if (i < n) f[st] += (cnt + 1) / (long double)n * dfs(st | 1<<i);
            // printf("At %d, Add %Lg(C(%d,2)/n)\n", i, (cnt * (cnt + 1) >> 1) / (long double)n, cnt);
        }
        // printf("dfs %X: %Lg\n", st, f[st]);
        for (int i = 0; i < n; i++) {
            curst = curst >> 1 | (curst & 1) << (n-1);
            f[curst] = f[st], vis[curst] = true;
        }
        return f[st];
    }
};
/*
枚举最后一个由 . 修改为 X 的断点，左右就可以分开为独立的两段，区间 DP 然后去合并就行了
f[][] 是当前状态下的 ans 在所有方案中的和，而 g[][] 自然是方案数。
我们记 z[l,r] 为 l~r 中 0 的数量。
比如现在做的是 f[l][r]，断点为 p \in [l,r]，转移为
g[l][r] += C(z[l,r]-1, z[l,p-1]) * g[l][p-1] * g[p+1][r] * (p - l + 1) ,
           {左右两边的操作自由组合}                            {选择的k在[l,p]中才有贡献}
f[l][r] += C(z[l,r]-1, z[l,p-1]) * g[l][p-1] * g[p+1][r] * (p-l)*(p-l+1)/2 + f[l][p-1] * g[p+1][r] * (p - l + 1) + f[p+1][r] * g[l][p-1] * (p - l + 1)
           {<-----          所有的方案数            --->}    {贡献是0+1+...+p-l} {<-----               前面的贡献乘上方案数                         ------>}

因为最终 len=n 会算重，所以最后一组单独计算，把最后一个 . 填成 X
*/
long double C[N][N], f[N][N], g[N][N];
int dotsum[N];
inline int z(int l, int r) { return dotsum[r] - dotsum[l-1]; }
int main() {
    for (int i = 0; i <= 200; i++) {
        C[i][0] = C[i][i] = 1;
        for (int j = 1; j < i; j++)
            C[i][j] = C[i-1][j-1] + C[i-1][j];
    }
    int Tc;
    scanf("%d", &Tc);
    while (Tc--) {
        scanf("%s", s+1);
        n = strlen(s+1);
        if (n <= 15 && false) {
            mask = (1<<n) - 1;
            BF sol{};
            sol.vis[mask] = true;
            uint16_t st = 0;
            for (int i = 0; i < n; i++)
                if (s[i+1] == 'X') st |= 1 << i;
            printf("%.15Lg\n", sol.dfs(st));
        } else {
            memcpy(s+n+1, s+1, n);
            // printf("%*s\n", n*2, s+1);
            for (int i = 1; i <= 2*n; i++)
                dotsum[i] = dotsum[i-1] + (s[i] == '.');
            memset(f, 0, sizeof f);memset(g, 0, sizeof g);
            for (int i = 1; i <= 2*n+1; i++)
                g[i][i-1] = 1; // 初始方案 
            for (int len = 1; len <= n; len++) {
                for (int i = 1, j = len; j <= 2*n; i++, j++) {
                    f[i][j] = 0, g[i][j] = 0;
                    int zij_1 = z(i, j) - 1, zlp1 = 0;
                    if (zij_1 < 0) { g[i][j] = 1; continue; }
                    for (int k = i; k <= j; k++) if (s[k] == '.') {
                        f[i][j] += C[zij_1][zlp1] * (
                            g[i][k-1] * g[k+1][j] * ((k-i)*(k-i+1)/2) +
                            (k-i+1) * (f[i][k-1] * g[k+1][j] + g[i][k-1] * f[k+1][j])
                        );
                        g[i][j] += C[zij_1][zlp1] * g[i][k-1] * g[k+1][j] * (k-i+1);
                        /* fprintf(stderr, "f,g[%d][%d] trans from %d, += C(%d,%d) * (%Lg*%Lg*%d + %d*(%Lg*%Lg+%Lg*%Lg)) = %Lg, %Lg\n", 
                            i, j, k, zij_1, zlp1, 
                            g[i][k-1], g[k+1][j], (k-i)*(k-i+1)/2,
                            (k-i+1), f[i][k-1], g[k+1][j], g[i][k-1], f[k+1][j], C[zij_1][zlp1] * (
                            g[i][k-1] * g[k+1][j] * ((k-i)*(k-i+1)/2) +
                            (k-i+1) * (f[i][k-1] * g[k+1][j] + g[i][k-1] * f[k+1][j])
                        ), C[zij_1][zlp1] * g[i][k-1] * g[k+1][j] * (k-i+1)); */
                        ++zlp1;
                    }
                    // printf(" (%Lg,%Lg)", f[i][j], g[i][j]);
                }
                // putchar('\n');
            }
            long double ans = 0.;
            for (int i = 1; i <= n; i++) {
                if (s[i] == '.') ans += f[i+1][i+n-1] * n + g[i+1][i+n-1] * (n*(n-1)/2);
                // ans += f[i][i+n-1];
            }
            for (int cnt = dotsum[n]; cnt--; ) ans /= n;
            printf("%.15Lg\n", ans);
        }
    }
    return 0;
}
