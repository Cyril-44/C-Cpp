#include <stdio.h>
int main() {
    int t, n, k;
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d", &n, &k);
        puts(((n - (n & 1 ^ 1)) - (n-k+1 + (n-k+1 & 1 ^ 1)) >> 1) & 1 ? "YES" : "NO");   // n-k+1 ~ n
    }
    return 0;
}