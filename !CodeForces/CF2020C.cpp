#include <stdio.h>
#include <string.h>
const int N = 64;
int a[N][2], pre[N][2];
int main() {
    int t, cur, curb, curc, curd;
    long long ans, b, c, d;
    scanf("%d", &t);
    while (t--) {
        scanf("%lld%lld%lld", &b, &c, &d);
        memset(a, -1, sizeof a);
        memset(pre, -1, sizeof pre);
        for (int i = 62; i >= 0; i--) {
            if (b & 1ll << i) curb = 1;
            else curb = -1;
            if (c & 1ll << i) curc = -1;
            else curc = 0;
            if (d & 1ll << i) curd = 1;
            else curd = 0;
            if (curb == 1 && curc == 0) {
                if (curd == 0) {
                    a[i][1] = 0, pre[i][1] = 0; // 借一位 1-1 - 0 = 0
                }
                else {
                    a[i][0] = 1, pre[i][0] = 0; // 不借 1 - 0 = 1
                }
            }
            else if (curb == 1 && curc == -1) {
                if (curd == 1) {
                    a[i][0] = 0, pre[i][0] = 0; // 1 - [0] = 1
                    a[i][1] = 1, pre[i][1] = 1; // 1+2-1 - [1] = 1
                }
                else {
                    a[i][0] = 1, pre[i][0] = 0; // 1 - [1] = 0
                    a[i][1] = 0, pre[i][1] = 0; // 1-1 - [0] = 0
                }
            }
            else if (curc == 0 && curb == -1) {
                if (curd == 0) {
                    a[i][0] = 0, pre[i][0] = 0; // [0] - 0 = 0
                    a[i][1] = 1, pre[i][1] = 0; // [1]-1 - 0 = 0
                }
                else {
                    a[i][0] = 1, pre[i][0] = 0; // [1] - 0 = 1
                    a[i][1] = 0, pre[i][1] = 1; // [0]+2-1 - 0 = 1
                }
            }
            else {
                if (curd == 0) {
                    a[i][0] = 0, pre[i][0] = 0; // [0] - [0] = 0
                }
                else {
                    a[i][1] = 0, pre[i][1] = 1; // [0]+2-1 - [0] = 1
                }
            }
        }
        ans = 0, cur = 0;
        for (int i = 0; i <= 62; i++) {
            if (cur == -1 || a[i][cur] == -1) {
                ans = -1;
                break;
            }
            else {
                ans |= (long long)(a[i][cur]) << i;
                cur = pre[i][cur];
            }
        }
        printf("%lld\n", ans);
    }
    return 0;
}