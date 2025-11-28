#include <stdio.h>
#include <algorithm>
const int N = 100005;
int c[N];
inline int gcd(int x, int y) {
    int z;
    while (y) z = x, x = y, y = z % y;
    return x;
}
int main() {
    int t, n, a, b, mx, g, ans;
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d%d", &n, &a, &b);
        g = gcd(a, b);
        for (int i = 0; i < n; i++) {
            scanf("%d", &c[i]);
            c[i] %= g;
        }
        std::sort(c, c + n);
        ans = c[n-1] - c[0];
        for (int i = 1; i < n; i++) {
            c[i-1] += g;
            if (c[i-1] - c[i] < ans)
                ans = c[i-1] - c[i];
        }
        printf("%d\n", ans);
    }
    return 0;
}