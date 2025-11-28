#include <stdio.h>
int main() {
    int t, n;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        while (--n) putchar('0');
        puts("1");
    }
    return 0;
}