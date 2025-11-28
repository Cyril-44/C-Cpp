#include <stdio.h>
int main() {
    int t, n, x1, x2;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        if (n ^ 2) {
            puts("No");
            while (n--) scanf("%*d", &n);
        }
        else {
            scanf("%d %d", &x1, &x2);
            if (x2 - x1 ^ 1) puts("Yes");
            else puts("No");
        }
    }
    return 0;
}