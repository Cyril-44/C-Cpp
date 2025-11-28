/*
0 1 1 0 1 1 1 1 0 1 1 1 1 1 1 0 ...
*/
#include <stdio.h>
#include <math.h>
int main() {
    int t;
    long long k, grp;
    scanf("%d", &t);
    while (t--) {
        scanf("%lld", &k);
        grp = sqrt(k);
        if (grp * (grp + 1) < k) ++grp;
        printf("%lld\n", k + grp);
    }
    return 0;
}