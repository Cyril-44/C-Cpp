#include <stdio.h>
int main() {
    int Q;
    long long x, y, z;
    scanf("%d", &Q);
    while (Q--) {
        scanf("%lld%lld%lld", &x, &y, &z);
        bool flg = false;
        for (int k = 0; k <= 32; k++) {
            long long a = x >> k+1 << k+1 | 1ll << k, b = x >> k+1 << k+1 | ((x & (1ll << k + 1) - 1) - (1ll << k));
            if (a - b <= y && a + b <= z)
                { flg = true; break; }
        }
        puts(flg ? "YES" : "NO");
    }
    return 0;
}