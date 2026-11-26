#include <bits/stdc++.h>
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    std::vector<std::vector<int>> a(n+1, std::vector<int>(m+1));
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            scanf("%d", &a[i][j]);
    int64_t ans = 0;
    for (int j = 1; j <= m; j++) {
        int64_t pre = 0;
        for (int i = 1; i <= n; i++) {
            pre += a[i][j-1];
            int x = a[i][j];
            if (x > pre) x -= pre, pre = 0;
            else pre -= x, x = 0;
            ans += x;
        }
    }
    printf("%ld\n", ans);
}