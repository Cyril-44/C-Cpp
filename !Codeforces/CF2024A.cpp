#include <stdio.h>
int main() {
    int t, a, b;
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d", &a, &b);
        if (a >= b) printf("%d\n", a);
        else if (b-a <= a) printf("%d\n", a - (b-a));
        else puts("0");
    }
    return 0;
}