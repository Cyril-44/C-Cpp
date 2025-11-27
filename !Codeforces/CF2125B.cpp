#include <cstdio>
#include <cstring>
#include <algorithm>
int main() {
    int t;
    long long a, b, k;
    scanf("%d", &t);
    while (t--) {
        scanf("%lld%lld%lld", &a, &b, &k);
        long long x = std::__gcd(a, b);
        long long y = std::max(a/x, b/x);
        if (k >= y) puts("1");
        else puts("2");
    }
    return 0;
}