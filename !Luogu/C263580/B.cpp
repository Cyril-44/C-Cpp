#include <cstdio>
#include <cstring>
#include <algorithm>
const int N = 200005;
int a[N];
int main() {
    int n, s;
    scanf("%d%d", &n, &s);
    for (int i = 1; i <= n; i++) scanf("%d", a + i);
    std::sort(a+1, a+1 + n);
    long long ans = (n & 1) ? -s : s;
    for (int i = (n>>1) + 1; i <= n; i++) ans += a[i] * 2ll;
    for (int i = 1; i <= (n>>1); i++) ans -= a[i] * 2ll;
    printf("%lld\n", ans);
    return 0;
}