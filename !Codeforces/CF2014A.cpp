#include <stdio.h>
int main() {
    int t, n, k, x, sum, ans;
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d", &n, &k);
        ans = sum = 0;
        while (n--) {
            scanf("%d", &x);
            if (x >= k) sum += x;
            else if (!x && sum)
                ++ans, --sum;
        }
        printf("%d\n", ans);
    }
    return 0;
}