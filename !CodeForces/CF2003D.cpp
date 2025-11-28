#include <stdio.h>
#include <vector>
#include <algorithm>
const int N = 200005;
int f[N][2];
int main() {
    int t, n, m, l, x, tmp, mx;
    long long ans;
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d", &n, &m);
        std::vector<int> a[n];
        for (int i = 0; i < n; i++) {
            scanf("%d", &l);
            while (l--) {
                scanf("%d", &x);
                a[i].emplace_back(x);
            }
        }
        mx = 0;
        for (int i = 0, j; i < n; i++) {
            std::sort(a[i].begin(), a[i].end());
            if (a[i].front() > 1) continue;
            for (j = 1; j < a[i].size(); j++)
                if (a[i][j] - a[i][j-1] > 1)
                    break;
            tmp = a[i][j-1] + 1;
            if (!a[i].front()) {
                if (a[i][j] - a[i][j-1] == 2) {
                    tmp = a[i][j] + 1;
                    if (a[i][j+1] - a[i][j] <= 1) {
                        for (++j; j < a[i].size(); j++)
                            if (a[i][j] - a[i][j-1] > 1) {
                                tmp = a[i][j-1] + 1;
                                break;
                            }
                        if (j == a[i].size()) tmp = a[i][j-1] + 1;
                    }
                }
                else if (a[i][j] - a[i][j-1] == 3)
                    tmp = a[i][j-1] + 2;
            }
            mx = std::max(mx, tmp);
        }
        ans = mx * (std::min(mx, m) + 1ll);
        if (mx < m) ans += (mx + 1ll + m) * (m - mx) / 2;
        printf("%lld\n", ans);
    }
    return 0;
}