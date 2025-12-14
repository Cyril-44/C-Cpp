#include <stdio.h>
const int N = 100005;
int a[N];
int main() {
    int t, n, m, l, r;
    long long mx;
    char c;
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d", &n, &m);
        mx = 0;
        for (int i = 1; i <= n; i++) {
            scanf("%d", &a[i]);
            if (a[i] > mx) mx = a[i];
        }
        while (m--) {
            scanf(" %c%d%d", &c, &l, &r);
            if (l <= mx && mx <= r) {
                switch (c) {
                    case '+': ++mx; break;
                    case '-': --mx; break;
                }
            }
            printf("%lld ", mx);
        }
        putchar('\n');
    }
    return 0;
}