#include <stdio.h>
const int N = 100005;
int a[N], b[N];
int main() {
    int t, n;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        for (int i = 0; i < n; i++)
            scanf("%d", &a[i]);
        int pl = n / 2 - 1, pe = n >> 1;
        if (n & 1) b[pe++] = a[n-1];
        for (int i = 0; (i<<1) < n; i++)
            b[pe++] = a[i], b[pl--] = a[n-i-(n&1)-1];
        for (int i = 0; i < n; i++)
            printf("%d ", b[i]);
        putchar('\n');
    }
    return 0;
}