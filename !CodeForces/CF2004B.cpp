#include <stdio.h>
int main() {
    int t, l1, r1, l2, r2;
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d %d%d", &l1, &r1, &l2, &r2);
        if (l1 > l2) l1 ^= l2 ^= l1 ^= l2, r1 ^= r2 ^= r1 ^= r2;
        else if (l1 == l2 && r1 > r2) r1 ^= r2 ^= r1 ^= r2;
        if (r1 < l2) puts("1");
        else if (r1 <= r2) printf("%d\n", r1 - l2 + (l1 != l2) + (r1 != r2)); // l1 <= l2 < r1 <= r2
        else printf("%d\n", r2 - l2 + 2); // l1 < l2 < r2 < r1
    }
    return 0;
}