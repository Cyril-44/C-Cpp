#include <stdio.h>
#include <iostream>
const int N = 200005;
int a[N], mnl[N], mnr[N];
long long s[N];
int main() {
    int t, n, ans;
    long long tp1, tp2;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        for (int i = 1; i <= n; i++)
            scanf("%d", &a[i]);
        for (int i = 1; i <= n; i++)
            s[i] = s[i-1] + a[i];
        /* mnl[1] = a[1];
        for (int i = 2; i <= n; i++)
            mnl[i] = std::min(mnl[i-1], a[i]);
        mnr[n] = a[n];
        for (int i = n-1; i >= 1; i--)
            mnr[i] = std::min(mnr[i+1], a[i]); */
        ans = 0;
        for (int i = 1; i <= n; i++) {
            tp1 = s[i-1] - i * (i - 1ll) / 2;
            tp2 = s[n] - s[i] - (n-i) * (n-i - 1ll) / 2;
            if (tp1 + tp2 >= n + std::min(i-1, n-i)) ++ans;
        }
        printf("%d\n", ans);
    }
    return 0;
}