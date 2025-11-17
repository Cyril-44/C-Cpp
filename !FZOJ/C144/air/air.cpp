#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <climits>
#include <vector>
constexpr int N = 1000005, MOD = 998244353;

#ifdef DEBUG
int a[20], tot, h[20];
void dfs(int i, int n, int m) {
    if (!n && !m) {
        int now = 0, mx = 0;
        for (int i = 1; i <= tot; i++)
            mx = std::max(mx, now += a[i]);
        ++h[mx];
        return;
    }
    if (n) a[i] = 1, dfs(i+1, n-1, m);
    if (m) a[i] = -1, dfs(i+1, n, m-1);
}
#endif

inline void add(int &x, int y) { if ((x += y) >= MOD) x -= MOD; }

namespace Pts50 {
/*
0 1 (1,0)
1 1 
2 1 
3 1 
4 1 

0 0 1 (2,0)
0 2 1 
2 3 1 
5 4 1 
9 5 1 
14 6 1 
20 7 1 

0 0 0 1 (3,0)
0 0 3 1 
0 5 4 1 
5 9 5 1 
14 14 6 1 
28 20 7 1 
48 27 8 1 
75 35 9 1 

0 0 0 0 1 (4,0)
0 0 0 4 1 
0 0 9 5 1 
0 14 14 6 1 
14 28 20 7 1 
42 48 27 8 1 
90 75 35 9 1 
165 110 44 10 1 

0 0 0 0 0 1 (5,0)
0 0 0 0 5 1 
0 0 0 14 6 1 
0 0 28 20 7 1 
0 42 48 27 8 1 
42 90 75 35 9 1 
132 165 110 44 10 1 
297 275 154 54 11 1 
572 429 208 65 12 1 
*/
    inline void solve(int n, int m) {
        std::vector<std::vector<int>> f(std::max(n+1,m+1), std::vector<int>(n+1));
        std::vector<int> g(n+1);
        f[n][n] = 1; g[0] = 1;
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= i; j++)
                add(g[j], g[j - 1]);
        for (int i = 0; i <= n; i++)
            f[i][n - i] = g[i];
        for (int i = 1; i <= m; i++) {
            f[i][n] = 1;
            for (int j = std::max(0, n-i + 1); j < n; j++)
                add(f[i][j] = f[i-1][j], f[i-1][j+1]);
        }
        int ans = 0;
        for (int i = 0; i <= n; i++)
            add(ans, 1ll * f[m][i] * i % MOD);
        // for (int i = 0; i <= m; i++)
        //     for (int j = 0; j <= n; j++)
        //         printf("%d%c", f[i][j], j==n?'\n':' ');
        printf("%d\n", ans);
    }
}

int main() {
    freopen("air.in", "r", stdin);
    freopen("air.out", "w", stdout);
    int n, m;
    scanf("%d%d", &n, &m);
#if defined(DEBUG) && false
start:
    tot = n+m;
    memset(h, 0, sizeof h);
    dfs(1, n, m);
    for (int i = 0; i <= n; i++)
        printf("%d ", h[i]);
    putchar('\n');
    scanf("%d%d", &n, &m);
    if (n != -1) goto start;
#endif
    if (m == 0) printf("%d\n", n);
    else if (1ll * n * m <= 30000000) Pts50::solve(n, m);
    return 0;
}
