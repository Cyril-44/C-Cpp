#include <cstdio>
#include <algorithm>
const int N = 100005;
int a[N];
int main() {
    int t, k, n;
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d", &n, &k);
        for (int i = 1; i <= n; i++)
            scanf("%d", &a[i]);
        int cnt = 0, ans = 0;
        for (int i = 1; i <= n; i++) {
            if (!a[i]) ++cnt;
            else cnt = 0;
            if (cnt == k) ++ans, cnt = -1;
        }
        printf("%d\n", ans);
    }
    return 0;
}