#include <stdio.h>
const int N = 32;
int a[N + 5];
int main() {
    int t, n, k, ans;
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d", &n, &k);
        if (k == 1) printf("%d\n", n);
        else {
            ans = 0;
            while (n) {
                ans += n % k;
                n /= k;
            }
            printf("%d\n", ans);
        }
    }
    return 0;
}