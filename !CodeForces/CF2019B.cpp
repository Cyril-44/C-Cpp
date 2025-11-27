#include <stdio.h>
#include <map>
const int N = 100005;
int a[N];
int main() {
    int t, n, q, p;
    long long k;
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d", &n, &q);
        for (int i = 0; i < n; i++)
            scanf("%d", &a[i]);
        std::map<long long, long long> mp;
        ++mp[n - 1ll];
        for (int i = 1; i < n; i++) {
            mp[i * 1ll * (n - i)] += a[i] - a[i-1] - 1;
            ++mp[i * 1ll * (n - i + 1) - 1ll];
        }
        while (q--) {
            scanf("%lld", &k);
            printf("%lld ", mp[k]);
        }
        putchar('\n');
    }
    return 0;
}