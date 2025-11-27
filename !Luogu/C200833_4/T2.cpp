#include <stdio.h>
int main() {
    int t, n, an;
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d", &n, &an);
        if (n & 1) printf("%d 1\n", an);
        else printf("1 %d\n", an);
    }
    return 0;
}