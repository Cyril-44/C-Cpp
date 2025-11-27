#include <stdio.h>
const int N = 100005;
int main() {
    int n, x;
    long long p;
    scanf("%d%lld", &n, &p);
    for (int i = 0; i < n; i++) {
        scanf("%d", &x);
        if (x > 0) p += x;
    }
    printf("%lld\n", p);
    return 0;
}