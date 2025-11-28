#include <stdio.h>
#include <algorithm>
const int N = 200005;
int a[N];
int main() {
    int t, n, k, p;
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d", &n, &k);
        for (int i = 1; i <= n; i++)
            scanf("%d", &a[i]);
        std::sort(a+1, a+1 + n);
        long long cur = 0;
        for (p = 1; p <= n; ++p) {
            if (cur + (a[p] - a[p-1]) * 1ll * (n - p + 1) >= k) break;
            cur += (a[p] - a[p-1]) * 1ll * (n - p + 1);
        }
        printf("%d\n", k + p-1);
    }
    return 0;
}