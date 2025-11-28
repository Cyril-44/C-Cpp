#include <stdio.h>
int main() {
    int t, x, y, k, a, b;
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d%d", &x, &y, &k);
        a = (x + k - 1) / k;
        b = (y + k - 1) / k;
        if (a == b) printf("%d\n", a + b);
        else if (a < b) printf("%d\n", b << 1);
        else printf("%d\n", a * 2 - 1);
    }
    return 0;
}