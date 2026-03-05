#include <stdio.h>
#include <algorithm>
constexpr int N = 5005, MOD = 998244353;
int a[N], pw2[N], C[N][N];
inline void add(int &x, int y) { if ((x += y) >= MOD) x -= MOD; }
int main() {
    pw2[0] = 1;
    for (int i = 1; i ^ N; i++) add(pw2[i] = pw2[i-1], pw2[i-1]);
    for (int i = 0; i ^ N; i++) {
        C[i][0] = C[i][i] = 1;
        for (int j = 1; j < i; j++)
            add(C[i][j] = C[i-1][j-1], C[i-1][j]);
    }
    int Tid, T, n, m;
    scanf("%d%d", &Tid, &T);
    while (T--) {
        scanf("%d%d", &n, &m);
        for (int i = 1; i <= n; i++)
            scanf("%d", &a[i]);
        std::sort(a, a+1 + n);
        int ans = 0;
        for (int j = 0; j < n; j++)
            for (int i = j+1; i <= n; i++) {
                // 找方案使得 存在 k ，ak/2 性价比紧跟在 ai 之后，ak/2 ~ aj 之间的所有 wi=2，且 ai+aj < ak
                int st = std::upper_bound(a+1, a+1+n, a[i]+a[j]) - a,
                    ed = std::lower_bound(a+1, a+1+n, 2*a[i]) - a;
                for (int k = st; k < ed; k++) {
                    // k w=2, (k, ed) w 都是 1, [j, i] w 也都是 1
                    int used = ed - k;
                    if (n - i >= used && m >= (2 - !j))
                        add(ans, 1ull * C[n - i - used][m - (2 - !j)] * (j>0?pw2[j-1]:1) % MOD);
                }
            }
        printf("%d\n", (pw2[n] - ans + MOD) % MOD);
    }
    return 0;
}