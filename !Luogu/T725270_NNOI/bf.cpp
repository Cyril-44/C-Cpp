#include <stdio.h>
constexpr int N = 200005;
int a[N];
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
    long long cnt = 0;
    for (int i = 1; i <= n; i++)
        for (int l = 1; l <= n; l++) {
            int p1 = i - l, p2 = i + l;
            if (p1 <= 0 || p2 > n) break;
            if (a[p1] < a[i] && a[i] < a[p2]) ++cnt;
        }
    printf("%lld\n", cnt);
    return 0;
}