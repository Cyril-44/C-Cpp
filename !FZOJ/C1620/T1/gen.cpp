#include <bits/stdc++.h>
int main() {
    int n = 20000, m = 20000;
    printf("%d %d\n", n, m);
    for (int i = 1; i <= n; i++) printf("%lld%c", (int64_t)1e14 - i, " \n"[i==n]);
    for (int i = 1; i <= n; i++) printf("%lld%c", (int64_t)1e14 - i, " \n"[i==n]);
}