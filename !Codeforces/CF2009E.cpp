#include <stdio.h>
#include <math.h>
inline long long min(const long long &x, const long long &y) {
    return x < y ? x : y;
}
inline long long sum(const long long &l, const long long &r) {
    return (r - l + 1ll) * (l + r) / 2;
}
int main() {
    int t, n, k;
    long long l, r, mid;
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d", &n, &k);
        for (l = k, r = k + n - 1; l <= r; ) {
            mid = (l+r) / 2;
            if (sum(k, mid) > sum(mid+1, k+n-1)) r = mid - 1;
            else l = mid + 1;
        }
        if (l == k+n-1) --l;
        if (r == k+n-1) --r;
        printf("%lld\n", min(abs(sum(k, l) - sum(l+1, k+n-1)), 
            abs(sum(k, r) - sum(r+1, k+n-1))));
    }
    return 0;
}