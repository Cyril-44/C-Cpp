#include <stdio.h>
const int N = 200005;
int a[N];
int main() {
    int t, n, mxid, cnt;
    long long sum = 0, l, r, mid;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        sum = mxid = 0;
        for (int i = 0; i < n; i++) {
            scanf("%d", &a[i]);
            if (a[mxid] < a[i]) mxid = i;
            sum += a[i];
        }
        if (n <= 2) puts("-1");
        else {
            l = 0, r = sum * 1ll * n + 1ll;
            while (l <= r) {
                mid = l + r >> 1;
                cnt = 0;
                for (int i = 0; i < n; i++)
                    if ((i ^ mxid) && a[i] * 2ll * n < sum + mid)
                        ++cnt;
                if ((cnt<<1) > n) r = mid - 1;
                else l = mid + 1;
            }
            printf("%lld\n", l);
        }
    }
    return 0;
}