#include <cstdio>
#include <algorithm>
#include <cstring>
const int N = 300005;
int a[N];
long long s[N];
int main() {
    int n, q;
    scanf("%d%d", &n, &q);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    std::sort(a+1, a+1+n);
    for (int i = 1; i <= n; i++)
        s[i] = s[i-1] + a[i];
    for (int b; q--; ) {
        scanf("%d", &b);
        int p = std::lower_bound(a+1, a+1+n, b) - a;
        if (p == n+1) puts("-1");
        else printf("%lld\n", s[p-1] + (b-1ll) * (n-p+1) + 1);
    }
    return 0;
}