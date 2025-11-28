#include <stdio.h>
const int N = 100005;
int a[N], f[N];
int main() {
    int t, n, cur, mx, x;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        cur = 0;
        for (int i = 1; i <= n; i++) {
            scanf("%d", &a[i]);
            if (a[i] > cur) f[i] = -1;
            else if (a[i] < cur) f[i] = 1;
            else f[i] = 0;
            cur -= f[i];
        }
        if (cur == n) {
            printf("%d\n", n - 1);
            continue;
        }
        x = 0, mx = 0;
        for (int i = 1; i <= n; i++) {
            x += f[i];
            if (x < 0) x = 0;
            if (x > mx) mx = x;
        }
        printf("%d\n", cur + mx);
    }
    return 0;
}