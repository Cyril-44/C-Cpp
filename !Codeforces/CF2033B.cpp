#include <stdio.h>
const int N = 505;
int a[N][N];
int main() {
    int t, n;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                scanf("%d", &a[i][j]);
        long long ans = 0;
        int mn1 = 0, mn2;
        for (int i = 0; i < n; i++)
            if (a[i][i] < mn1)
                mn1 = a[i][i];
        ans -= mn1;
        for (int i = 1, j; i < n; i++) {
            // (0, i) (i, 0)
            mn1 = mn2 = 0;
            for (int k = 0; i+k < n; k++) {
                j = i + k;
                if (a[j][k] < mn1) mn1 = a[j][k];
                if (a[k][j] < mn2) mn2 = a[k][j];
            }
            ans -= mn1 + mn2;
        }
        printf("%lld\n", ans);
    }
    return 0;
}