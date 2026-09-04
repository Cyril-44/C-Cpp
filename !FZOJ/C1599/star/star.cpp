#include <cstdio>
#include <cstdint>
#include <algorithm>
constexpr int N = 3004;
char s[N][N];
int sum[N][N];
#pragma GCC optimize("unroll-loops")
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    // if (n > 1000) return 1145141919;
    for (int i = 1; i <= n; i++)
        scanf("%s", s[i] + 1);
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            sum[i][j] = sum[i-1][j] + (s[i][j] == s[i-1][j]);
    int64_t ans = 0;
    for (int l = 1; l < n; l++)
        for (int r = l+1; r <= n; r++) {
            int cnt = 0; const int len = r - l;
            for (int i = 1; i <= m; i++) {
                if (s[l][i] != s[r][i]) { cnt = 0; continue; }
                if (s[l][i] != s[l][i-1]) cnt = 0;
                if (sum[r][i] - sum[l][i] == len) ans += cnt++;
            }
        }
    printf("%lld\n", ans);
    return 0;
}