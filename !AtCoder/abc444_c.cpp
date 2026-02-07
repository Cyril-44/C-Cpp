#include <stdio.h>
#include <algorithm>
constexpr int N = 300005;
int a[N];
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    std::sort(a+1, a+1 + n);
    if (n & 1) printf("%d\n", a[n]);
    else {
        int all = a[n];
        int j = n;
        while (j > 0 && a[j] == all) --j;
        if (j & 1 ^ 1) {
            for (int i = 1; i < j; i++, j--)
                if (a[i] + a[j] != all) { all = -1; break; }
            if (all != -1) printf("%d ", all);
        }
        all = a[1] + a[n];
        for (int i = 2, j = n-1; i < j; i++, j--)
            if (a[i] + a[j] != all) { all = -1; break; }
        if (all != -1) printf("%d ", all);
        putchar('\n');
    }
    return 0;
}