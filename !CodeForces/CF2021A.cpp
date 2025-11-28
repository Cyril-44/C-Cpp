#include <stdio.h>
#include <algorithm>
const int N = 55;
int a[N];
int main() {
    int t, n;
    long long ans;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        for (int i = 0; i < n; i++) {
            scanf("%d", &a[i]);
        }
        std::sort(a, a + n);
        ans = a[0];
        for (int i = 1; i < n; i++)
            ans = (ans + a[i]) / 2;
        printf("%lld\n", ans);
    }
    return 0;
}