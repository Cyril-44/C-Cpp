#include <bits/stdc++.h>
constexpr int N = 100005;
int a[N], s[N];
int main() {
    freopen("game.in", "r", stdin);
    freopen("game.out", "w", stdout);
    int n, k;
    scanf("%d%d", &n, &k);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]), s[i] = s[i-1] + a[i];
    if (n == 1) printf("%d\n", a[1]);
    else if (n == 2) {
        if (k) printf("%d\n", a[1] + a[2]);
        else printf("%d\n", std::min(a[1], a[2]));
    } else {
        int midp = (n + 1) / 2;
        if (k && n%2==0 || !k && n%2) {
            int ans = 1<<30;
            for (int i = 1; i <= midp; i++)
                ans = std::min(ans, s[i + n/2] - s[i-1]);
            printf("%d\n", ans);
        } else {
            if (std::accumulate(a+1, a+1+n, true, [](bool f, int x) { return f && a[x] == a[1]; }))
                printf("%d\n", a[1]);
            else {
                if (k) {
                    printf("%d\n", *std::min_element(a+1, a+1+n));
                } else {

                }
            }
        }
    }
}