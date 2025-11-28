#include <stdio.h>
#include <algorithm>
#include <functional>
const int N = 200005;
int a[N];
int main() {
    int t, n, k;
    long long now;
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d", &n, &k);
        for (int i = 0; i < n; i++)
            scanf("%d", a+i);
        std::sort(a, a + n, std::greater<int>());
        now = 0ll;
        for (int i = 0; i+1 < n; i += 2)
            now += a[i] - a[i+1];
        printf("%lld\n", (now - k < 0 ? 0 : now - k) + ((n & 1) ? a[n-1] : 0));
    }
    return 0;
}