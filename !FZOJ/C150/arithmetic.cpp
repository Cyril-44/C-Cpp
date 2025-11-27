#include <stdio.h>
constexpr int N = (int)1e5 + 5;
int a[N];
int main() {
    freopen("arithmetic.in", "r", stdin);
    freopen("arithmetic.out", "w", stdout);
    int n, cnt1, cnt2, mnp, __scanf_result;
    a[0] = (int)1e9;
start:
    __scanf_result = scanf("%d", &n);
    cnt1 = cnt2 = mnp = 0;
#ifdef DEBUG
    if (__scanf_result == -1) return 0;
#endif
    for (int i = 1; i <= n; i++) {
        scanf("%d", &a[i]);
        if (a[i] == 2) ++cnt2;
        if (a[i] == 1) ++cnt1;
        else if (a[i] < a[mnp]) mnp = i;
    }
    if (n == 1) return printf("%d\n", a[1]), 0;
    switch (cnt1) {
    case 0: // Direct Output
        for (int i = 1; i < n; i++)
            printf("%d*", a[i]);
        printf("%d\n", a[n]);
        break;
    case 1:
        for (int i = 1; i <= n; i++)
            if (a[i] != 1 && (i ^ mnp)) printf("%d*", a[i]);
        printf("(%d+1)\n", a[mnp]);
        break;
    default:
        for (int i = 1; i <= n; i++)
            if (a[i] > 2) printf("%d*", a[i]);
        if (cnt1 <= cnt2) { // Get 3
            while (cnt2-- > cnt1) printf("2*");
            while (cnt2--, cnt1-- > 1) printf("(1+2)*");
            puts("(1+2)");
            break;
        }
        while (cnt1 > 2 && cnt2 > 1)
            printf("(1+2)*"), --cnt1, --cnt2;
        if (cnt1 == 2 && cnt2 == 1) {
            puts("2*(1+1)");
            break;
        }
        if (cnt2 > 0) {
            --cnt1, --cnt2;
            printf("(1+2)");
            if (cnt1) putchar('*');
            else putchar('\n');
        }
        while (cnt1 > 0) {
            switch (cnt1) {
                case 2: puts("(1+1)"); cnt1 = 0; break;
                case 3: puts("(1+1+1)"); cnt1 = 0; break;
                case 4: puts("(1+1)*(1+1)"); cnt1 = 0; break;
                default: printf("(1+1+1)*"), cnt1 -= 3;
            }
        }
    }
#ifdef DEBUG
    goto start;
#endif
    return 0;
}