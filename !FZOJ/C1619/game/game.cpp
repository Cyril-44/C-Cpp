#include <bits/stdc++.h>
constexpr int N = 100005;
int a[N], s[N], n;
inline bool check(int s) {
    int v = 0, cnt = 0;
    for (int i = 1; i <= n; i++) {
        if ((v += a[i]) >= s) v = 0, ++cnt;
    }
    return cnt > n/2;
}
int main() {
    int k;
    scanf("%d%d", &n, &k);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]), s[i] = s[i-1] + a[i];
    if (n == 1) printf("%d\n", a[1]);
    else if (n == 2) {
        if (k) printf("%d\n", a[1] + a[2]);
        else printf("%d\n", std::min(a[1], a[2]));
    } else {
        if (k && n%2==0 || !k && n%2) {
            const int midp = (n + 1) / 2;
            int ans = 1<<30;
            for (int i = 1; i <= midp; i++)
                ans = std::min(ans, s[i + n/2] - s[i-1]);
            printf("%d\n", ans);
        } else {
            int l = 0, r = s[n];
            while (l <= r) {
                int mid = l + r >> 1;
                if (check(mid)) l = mid + 1;
                else r = mid - 1;
            }
            printf("%d\n", r);
        }
    }
}