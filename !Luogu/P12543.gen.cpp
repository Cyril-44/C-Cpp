#include <bits/stdc++.h>
int main() {
    int n;
    scanf("%d", &n);
    printf("%d\n", n);
    // for (int i = 0; i < n/2; i++)
    //     printf("0 ");
    // for (int i = n/2; i < n; i++)
    //     printf("25000 ");
    for (int i = 0; i < n; i++)
        printf("%d ", 50000 * i / n);
    putchar('\n');
    return 0;
}
