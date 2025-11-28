#include <stdio.h>
/*
n = k * q + r (r < q)
*/
int main() {
#ifndef ONLINE_JUDGE
    freopen("division.in", "r", stdin);
    freopen("division.out", "w", stdout);
#endif
    int t;
    long long n, k, q, r;
    scanf("%d", &t);
    while (t--) {
        scanf("%lld %lld", &n, &k);
        if (k == 0) puts("1");
        else {
            q = n / k, r = n % k;
            if (q <= r) puts("0");
            else printf("%lld\n", (q - r + k) / (k+1));
        }
    }
    return 0;
}