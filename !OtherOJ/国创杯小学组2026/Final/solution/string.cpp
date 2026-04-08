#include <cstdio>
#include <cstring>
#include <algorithm>
constexpr int N = 5000005;
int f[N], a[N][3], g[N];
char s[N];
int main() {
    freopen("string.in", "r", stdin);
    freopen("string.out", "w", stdout);
    int T, n, m;
    scanf("%*d%d", &T);
    while (T--) {
        scanf("%s", s+1);
        n = strlen(s+1);

        m = 0;
        for (int i = 1; i <= n; i++) {
            if (s[i] == s[i-1]) ++a[m][1];
            else a[++m][0] = s[i], a[m][1] = 1, a[m][2] = i;
        }

        memset(g, 0, sizeof(int) * (n+1));
        for (int i = 1, j, k; i <= m; i++) {
            j = a[i][2] - 1, k = a[i][2] + a[i][1];
            while (j > 0 && k <= n && s[j] == s[k] && s[j] <= s[j+1]) g[k++] = j--;
        }
        for (int i = 1; i <= m; i++) {
            int mnf = f[a[i][2]-1];
            for (int j = a[i][2]; j - a[i][2] < a[i][1]; j++) {
                f[j] = mnf + 1;
                if (g[j]) mnf = std::min(mnf, f[j] = std::min(f[j], f[g[j] - 1] + 1));
            }
        }
        printf("%d\n", f[n]);
    }
    return 0;
}