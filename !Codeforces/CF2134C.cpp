#include <bits/stdc++.h>
using namespace std;
constexpr int N = 200005;
int a[N];
int main() {
    int T, n;
    scanf("%d", &T);
    while (T--) {
        scanf("%d", &n);
        for (int i = 1; i <= n; i++)
            scanf("%d", &a[i]);
        long long ans = 0;
        a[n+1] = 0;
        for (int i = 2; i <= n; i += 2) {
            if (a[i] < a[i-1]) ans += a[i-1] - a[i], a[i-1] = a[i];
            if (a[i] < a[i-1] + a[i+1]) {
                ans += a[i-1] + a[i+1] - a[i];
                a[i+1] -= a[i-1] + a[i+1] - a[i];
            }
        }
        printf("%lld\n", ans);
    }
    return 0;
}