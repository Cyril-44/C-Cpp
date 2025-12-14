#include <stdio.h>
const int N = 1005;
int main() {
    int t, n, ai, mn, mx;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        mn = N, mx = 0;
        for (int i = 0; i < n; i++) {
            scanf("%d", &ai);
            if (ai > mx) mx = ai;
            if (ai < mn) mn = ai;
        }
        printf("%d\n", (mx - mn) * (n-1));
    }
    return 0;
}