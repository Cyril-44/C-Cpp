#include <stdio.h>
const int N = 1000005;
int a[N], p[N];
inline void swap(int &x, int &y) {
    x ^= y ^= x ^= y;
}
int main() {
    int t, n, ans;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        for (int i = 1; i <= n; i++) {
            scanf("%d", &a[i]);
            p[a[i]] = i;
        }
        ans = 0;
        for (int i = 1; i <= n; i++) {
            if (a[i] ^ i) {
                if (a[a[i]] ^ a[i]) {
                    if (a[a[i]] ^ i) {
                        swap(a[a[i]], a[p[i]]); // a[i] <-> p[i]
                        ++ans;
                        swap(p[a[a[i]]], p[a[p[i]]]);
                    }
                }
                else {
                    swap(a[i], a[p[i]]);    // i <-> p[i]
                    ++ans;
                    swap(p[a[i]], p[a[p[i]]]);
                }
            }
        }
        printf("%d\n", ans);
    }
    return 0;
}