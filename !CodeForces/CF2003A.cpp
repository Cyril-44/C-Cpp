#include <stdio.h>
#include <string.h>
const int N = 105;
char a[N];
int main() {
    int t, n;
    scanf("%d", &t);
    while (t--) {
        scanf("%d %s", &n, a);
        if (*a == a[n-1]) puts("No");
        else puts("Yes");
    }
    return 0;
}