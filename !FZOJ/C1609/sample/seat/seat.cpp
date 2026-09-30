#include <cstdio>
int main() {
    int T = 10, n;
    while (T--) {
        scanf("%d", &n);
        if (n == 1) puts("2");
        else if (n == 2) puts("3");
        else if (n & 1) printf("%lld\n", (n/2+1ll) * (n/2+2ll) + 1);
        else printf("%lld\n", (n/2) * (n/2+2ll) + 1);
    }
    return 0;
}