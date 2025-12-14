#include <stdio.h>
const int N = 200005;
int a[N];
inline void swap(int &x, int &y) {
    x ^= y ^= x ^= y;
}
inline int abs(const int &x) {
    return x < 0 ? -x : x;
}
int main() {
    int t, n;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        for (int i = 1; i <= n; i++)
            scanf("%d", &a[i]);
        bool flg = true;
        for (int i = 1; i < n; i++) {
            if ((a[i] ^ i) && abs(a[i+1] - a[i]) == 1) {
                if (abs(a[i+1] - a[i]) == 1)
                    swap(a[i], a[i+1]);
            }
            if (a[i] ^ i) {
                flg = false;
                break;
            }
        }
        puts(flg ? "YES" : "NO");
    }
    return 0;
}