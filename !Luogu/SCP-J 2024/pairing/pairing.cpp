#include <stdio.h>
const int N = 500005;
int f[N][2];
__always_inline int max(const int &x, const int &y) {
    return x > y ? x : y;
}
int main() {
#ifndef ONLINE_JUDGE
    freopen("pairing.in", "r", stdin);
    freopen("pairing.out", "w", stdout);
#endif
    int n, ai, ans, mx, mxi, mx0, mxi0;
    ans = mx = mxi = mx0 = mxi0 = 0;
    scanf("%d", &n);
    while (n--) {
        scanf("%d", &ai);
        if (f[ai][0] & 1) f[ai][1] = max(f[ai][1], f[ai][0] + 1);
        if (mxi ^ ai) // 找不一样的
            f[ai][0] = max(f[ai][0], mx + 1);
        else f[ai][0] = max(f[ai][0], mx0 + 1);
        if (f[ai][1] > mx) {
            if (ai ^ mxi) {
                mx0 = mx, mxi0 = mxi;
                mx = f[ai][1], mxi = ai;
            }
            else mx = f[ai][1];
        }
        else if (f[ai][1] > mx0) {
            if (ai ^ mxi) { // 一定要注意这个细节
                mx0 = f[ai][1];
                mxi0 = ai;
            }
        }
        ans = max(ans, f[ai][1]);
    }
    printf("%d\n", ans > 1 ? ans : 0);
    return 0;
}