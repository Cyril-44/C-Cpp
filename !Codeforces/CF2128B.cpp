#include <cstdio>
#include <algorithm>
#include <cstring>
#ifndef ONLINE_JUDGE
# define debug(...) fprintf(stderr, __VA_ARGS__)
#else
# define debug(...) 0
#endif
const int N = 100005;
int a[N];
int main() {
    int t, n;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        for (int i = 1; i <= n; i++) scanf("%d", a + i);
        putchar('L'); putchar('L');
        int l = 3, r = n, last = 2;
        int down = a[1] > a[2];
        // debug("%d ", a[1]);
        for (int i = 3; i <= n; i++) {
            // debug("%d ", a[last]);
            if ((a[last] > a[l]) ^ down) putchar('L'), down ^= 1, last = l++;
            else if ((a[last] > a[r]) ^ down) putchar('R'), down ^= 1, last = r--;
            else if ((a[l] > a[r]) ^ down) putchar('L'), last = l++;
            else putchar('R'), last = r--;
        }
        // debug("%d\n", a[last]);
        putchar('\n');
    }
    return 0;
}