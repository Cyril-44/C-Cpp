#include <stdio.h>
const int N = 300005;
int a[N], f[N][2];
inline int max(const int &x, const int &y) {
    return x > y ? x : y;
}
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &a[i]);
        f[i][0] = a[i] - i;
        f[i][1] = i;
    }
    for (int i = n-2; i >= 0; i--) {
        if (f[i][0] <= f[i+1][0])
            f[i][0] = f[i+1][0], f[i][1] = f[i+1][1];
    }
    for (int i = 0; i < n; i++) {
        if (a[i] == 0) puts("1 0");
        else if (i + a[i] >= n) puts("0");
        else if (f[max(0, i + a[i])][0] + i >= 0) // a[i+j] - j >= 0
            printf("1 %d\n", f[max(0, i + a[i])][1] - i);
        else puts("0");
    }
    return 0;
}