#include <stdio.h>
#include <string.h>
const int N = 105;
int h[N];
int main() {
    int t, n, x, mx;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        memset(h, 0, n+1<<2);
        mx = 0;
        for (int i = 0; i < n; i++)
            scanf("%d", &x), ++h[x];
        for (int i = 1; i <= n; i++)
            if (h[i] > h[mx]) mx = i;
        printf("%d\n", n - h[mx]);
    }
    return 0;
}