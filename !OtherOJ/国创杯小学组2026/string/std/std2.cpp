#include <cstdio>
#include <cstring>
#include <algorithm>
constexpr int N = 5000005;
int f[N], sta[N], a[N][3], g[N];
char s[N];
int main() {
    int T, n, m, top;
    scanf("%d", &T);
    while (T--) {
        scanf("%s", s+1);
        n = strlen(s+1);

        m = 0;
        for (int i = 1; i <= n; i++) {
            if (s[i] == s[i-1]) ++a[m][1];
            else a[++m][0] = s[i], a[m][1] = 1, a[m][2] = i;
        }

        for (int i = 1, j, k; i <= m; i++) {
            j = a[i][2], k = a[i][2] + a[i][1] - 1;
            
        }
        top = 0;
        a[m+1][0] = 114514;
        printf("%d\n", f[n]);
    }
    return 0;
}