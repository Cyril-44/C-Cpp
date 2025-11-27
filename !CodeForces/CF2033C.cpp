#include <stdio.h>
const int N = 100005;
int a[N], f[N];
inline int min(const int &x, const int &y) {
    return x < y ? x : y;
}
int main() {
    int t, n;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        for (int i = 1; i <= n; i++)
            scanf("%d", &a[i]);
        for (int i = 2; (i<<1)-1 <= n; i++) {
            bool flg1 = a[i] == a[i - 1],
                 flg2 = a[i] == a[n - i + 2],
                 flg3 = a[n - i + 1] == a[i - 1],
                 flg4 = a[n - i + 1] == a[n - i + 2];
            f[i] = min(f[i-1] + flg1 + flg4, f[i-1] + flg2 + flg3);
        }
        printf("%d\n", (n&1) ? f[n+1 >> 1] : f[n >> 1] + (a[n>>1] == a[n+2 >> 1]));
    }
    return 0;
}