#include <bits/stdc++.h>
int main() {
    int n, k;
    scanf("%d%d", &n, &k);
    int lmt = std::min(n, k);
    int64_t res = 1ll * n * k;
    for (int l=1, r; l <= lmt; l=r+1) {
        r = std::min(k / (k/l), lmt);
        res -= 1ll * (k/l) * (r-l+1ll) * (l+r)/2;
    }
    printf("%lld\n", res);
    return 0;
}