#include <stdio.h>
int main() {
    int t, n;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        if (n & 1) {
            for (int i = 1; i <= n; i += 2)
                printf("%d ", i);
            for (int i = n-1; i >= 2; i -= 2)
                printf("%d ", i);
            putchar('\n');
        }
        else puts("-1");
    }
    return 0;
}
/*
3: 2 3 1
4: 2 3 4 1  X
5: 1 3 5 4 2
6: 1 3 6 5 4 2  X
*/
