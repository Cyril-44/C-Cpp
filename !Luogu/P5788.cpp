#include <stdio.h>
const int N = 3000005;
int a[N], sta[N], f[N];
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    int top(0);
    for (int i = 1; i <= n; i++) {
        while (top > 0 && a[sta[top]] < a[i])
            f[sta[top--]] = i;
        sta[++top] = i;
    }
    for (int i = 1; i <= n; i++)
        printf("%d ", f[i]);
    putchar('\n');
    return 0;
}