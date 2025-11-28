#include <cstdio>
#include <string.h>
const int N = 200005;
int a[N];
int main() {
    int t, n;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        memset(a, 0, sizeof(int) * (n+1));
        int cnt = 0;
        if (n & 1) {
            if (n < 26) {
                puts("-1");
                continue;
            }
            a[1] = a[10] = a[26] = ++cnt;
            if (n == 27) a[27] = a[23] = ++cnt; // patch
        }
        for (int i = 1; i <= n; i++) {
            if (a[i]) continue;
            if (a[i+1]) a[i] = a[i+4] = ++cnt;
            else a[i] = a[i+1] = ++cnt;
        }
        for (int i = 1; i <= n; i++)
            printf("%d ", a[i]);
        putchar('\n');
    }
    return 0;
}