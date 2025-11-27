#include <cstdio>
int main() {
    int t, x;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &x);
        int a[10]{};
        while (x) a[++a[0]] = x % 10, x /= 10;
        int ans = 10;
        for (int i = 1; i <= a[0]; i++)
            if (ans > a[i]) ans = a[i];
        printf("%d\n", ans);
    }
    return 0;
}