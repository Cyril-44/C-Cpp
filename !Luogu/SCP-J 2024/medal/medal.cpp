#include <stdio.h>
#include <algorithm>
#include <string.h>
using std::min;
const int N = 200005;
int s[N], g[N], b[N], n;
inline void unique(int *a) {
    const auto cmp = [] (const int &x, const int &y) { return y < x; };
    static int tmp[N];
    memcpy(tmp, a, sizeof tmp);
    std::sort(tmp, tmp + n, cmp);
    for (int i = 0; i < n; i++)
        a[i] = std::lower_bound(tmp, tmp + n, a[i], cmp) - tmp + 1;
}
int main() {
#ifndef ONLINE_JUDGE
    freopen("medal.in", "r", stdin);
    freopen("medal.out", "w", stdout);
#endif
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
        scanf("%d%d%d", &s[i], &g[i], &b[i]);
    unique(s), unique(g), unique(b);
    for (int i = 0; i < n; i++)
        printf("%d\n", min(min(s[i], g[i]), b[i]));
    return 0;
}