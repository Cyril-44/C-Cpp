#include <cstdio>
const int N = 200005;
int a[N];
int main() {
    int t, n, k;
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d", &n, &k);
        for (int i = 0; i < n; i++)
            scanf("%d", &a[i]);
        k -= 2;
        int p = 1, ans = 1, dans = 0x7fffffff;
        while (k >= 0 && p + k < n) {
            int cnt = 0;
            while (p + k < n && a[p] == ans) {
                ++p, ++cnt;
            }
            if (cnt >= 2 && dans == 0x7fffffff) dans = ans + 1;
            if (p + k < n && a[p] != ans) break;
            ++ans;
            k -= 2, ++p;
        }
        if (ans > dans) ans = dans;
        printf("%d\n", ans);
    }
    return 0;
}