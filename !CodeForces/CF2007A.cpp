#include <stdio.h>
int main() {
    int t, l, r;
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d", &l, &r);
        printf("%d\n", r - l + 1 + (l & 1) >> 2);
    }
    return 0;
}