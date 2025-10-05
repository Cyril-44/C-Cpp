#include <cstdio>
#include <algorithm>
#include <cstring>
int main() {
    int t, n;
    scanf("%d", &t);
    while (t--) {
        int a[10]{}, b[10]{};
        scanf("%d", &n);
        for (int i = 0; i < n; i++)
            scanf("%d", a+i);
        for (int i = 0; i < n; i++)
            scanf("%d", b+i);
        int ans = 0;
        for (int i = 0; i < n; i++)
            if (a[i] > b[i]) ans += a[i] - b[i];
        printf("%d\n", ans+1);
    }
    return 0;
}