#include <stdio.h>
#include <string.h>
const int N = 105;
int f[N][N], a[N];
inline int max(const int &x, const int &y) {
    return x > y ? x : y;
}
int main() {
    int t, n, ans;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        a[1] = 0;
        for (int i = 0; i < n; i++)
            scanf("%d", &a[i]);
        memset(f, 0, sizeof(int) * n * N);
        f[0][1] = a[0];
        f[1][1] = a[1];
        ans = max(a[0], a[1]) + 1;
        for (int i = 2; i < n; i++)
            for (int k = 0; k <= i/2; k++) {
                for (int j = i-2; j >= 0; j--)
                    if (f[j][k])
                        f[i][k+1] = max(f[i][k+1], max(a[i], f[j][k]));
                ans = max(ans, f[i][k+1] + k+1);
            }
        printf("%d\n", ans);
    }
    return 0;
}