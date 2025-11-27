#include <stdio.h>
const int N = 1000005;
int h[N], v[N];
int sta[N], f[N];
int main() {
    int n, top;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        scanf("%d%d", &h[i], &v[i]);
    top = 0;
    for (int i = 1; i <= n; i++) {
        while (top > 0 && h[sta[top]] <= h[i])
            --top;
        f[sta[top]] += v[i];
        sta[++top] = i;
    }
    top = 0;
    for (int i = n; i >= 1; i--) {
        while (top > 0 && h[sta[top]] <= h[i])
            --top;
        f[sta[top]] += v[i];
        sta[++top] = i;
    }
    int ans(0);
    for (int i = 1; i <= n; i++)
        ans = f[i] > ans ? f[i] : ans;
    printf("%d\n", ans);
    return 0;
}