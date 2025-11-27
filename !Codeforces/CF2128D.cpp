#include <cstdio>
#include <algorithm>
#include <cstring>
const int N = 500005;
int a[N], f[N];
long long s[N];
int main() {
    int t, n;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        a[0] = (int)1e9;
        for (int i = 1; i <= n; i++) scanf("%d", a + i);
        long long ans = 0;
        f[1] = 1, s[1] = 0;
        for (int i = 2; i <= n; i++) {
            if (a[i-2] > a[i]) f[i] = f[i-2] + 1, s[i] = s[i-2] + f[i-2]*2;
            if (a[i-1] > a[i]) f[i] = f[i-1] + 1, s[i] = s[i-1] + f[i-1];
        }
        for (int i = 1; i <= n; i++) {
            ans += f[i] * (long long)i - s[i];
        }
        printf("%lld\n", ans);
    }
    return 0;
}