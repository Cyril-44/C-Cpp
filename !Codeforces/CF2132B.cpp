#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
i64 ans[25];
int main() {
    int t;
    i64 n;
    bool outputed;
    scanf("%d", &t);
    while (t--) {
        scanf("%lld", &n);
        outputed = false;
        int cnt = 0;
        for (i64 base = 10; base <= n; base *= 10) {
            i64 l = 0, r = (i64)9e18 / base, mid;
            while (l <= r) {
                mid = l + (r - l >> 1);
                if (mid * base + mid < n) l = mid + 1;
                else r = mid - 1;
            }
            if (l * base + l == n) ans[cnt++] = l;
        }
        std::sort(ans, ans + cnt);
        printf("%d\n", cnt);
        if (cnt) {
            for (int i = 0; i < cnt; i++)
                printf("%lld ", ans[i]);
            putchar ('\n');
        }
    }
    return 0;
}