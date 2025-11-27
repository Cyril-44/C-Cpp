#include <bits/stdc++.h>
using namespace std;
constexpr int N = 200005;
int a[N];
int main() {
    int t, n;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        for (int i = 1; i <= n; i++)
            scanf("%d", &a[i]);
        int oddmn = 0x7fffffff, evenmx = 0;
        long long ans = 0;
        int sec = 0;
        for (int i = 1; i <= n; i++) {
            if (i & 1) {
                oddmn = min(oddmn, a[i] * 2 + i), sec = max(sec, evenmx - (a[i] * 2 - i)), ans += a[i];
            }
            else {
                evenmx = max(evenmx, a[i] * 2 - i), sec = max(sec, a[i] * 2 + i - oddmn), ans -= a[i];
            }
        }
        ans += max(sec, (n&1) ? (n-1) : (n-2));
        printf("%lld\n", ans);
    }
    return 0;
}