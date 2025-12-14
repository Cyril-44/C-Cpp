#include <stdio.h>
const int N = 55;
int a[N];
int main() {
    int t, n, ans;
    scanf("%d", &t);
    while (t--) {
        int h[N] = {};
        scanf("%d", &n);
        for (int i = 0; i < n; i++) {
            scanf("%d", &a[i]);
            ++h[a[i]];
        }
        ans = n;
        for (int i = 1; i <= n; i++)
            if (n - h[i] < ans)
                ans = n - h[i];
        printf("%d\n", ans);
    }
    return 0;
}