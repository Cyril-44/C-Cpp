#include <stdio.h>
const int N = 505;
char s[N][5];
int main() {
    int t, n;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        for (int i = 1; i <= n; i++)
            scanf("%s", s[i]);
        for (int i = n, j; i >= 1; i--) {
            for (j = 0; s[i][j]; j++)
                if (s[i][j] == '#') break;
            printf("%d ", j+1);
        }
        putchar('\n');
    }
    return 0;
}