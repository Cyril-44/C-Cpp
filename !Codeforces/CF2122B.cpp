#include <cstdio>
#include <cstring>
const int N = 200005;
int a[N], b[N], c[N], d[N];
int main() {
    int t, n;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        for (int i = 1; i <= n; i++)
            scanf("%d%d%d%d", a+i, b+i, c+i, d+i);
        long long ans = 0;
        for (int i = 1; i <= n; i++) {
            if (a[i] > c[i]) ans += a[i] - c[i], a[i] = c[i];
            if (b[i] > d[i]) ans += a[i] + b[i] - d[i];
        }
        printf("%lld\n", ans);
    }
    return 0;
}