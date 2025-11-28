#include <stdio.h>
int main() {
    int t, xc, yc, k;
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d%d", &xc, &yc, &k);
        if (k & 1) printf("%d %d\n", xc, yc);
        for (int i = 1; (i<<1) <= k; i++) {
            printf("%d %d\n", xc - i, yc - i);
            printf("%d %d\n", xc + i, yc + i);
        }
    }
    return 0;
}