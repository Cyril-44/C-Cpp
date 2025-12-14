#include <stdio.h>
#include <algorithm>
constexpr int N = 100005;
long long a[N], x[N];
int main() {
    freopen("queue.in", "r", stdin);
    freopen("queue.out", "w", stdout);
    int n, Q, D;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    std::sort(a+1, a+1 + n);
    for (int i = 1; i <= n; i++) a[i] -= i-1;
    a[n+1] = 1ll << 60;
    scanf("%d%d", &Q, &D);
    for (int q = 1; q <= Q; q++) {
        scanf("%lld", &x[q]);
        if (x[q] < a[1]) { printf("%lld\n", x[q]); continue; }
        long long d = D;
        while (d > 0) {
            int near = std::upper_bound(a+1, a+1 + n, x[q]) - a-1;
            long long todel = (a[near + 1] - x[q] + near - 1) / near;
            if (todel <= d) {
                d -= todel;
                x[q] += todel * near;
            } else x[q] += near * d, d = 0;
        }
        printf("%lld\n", x[q]);
    }
    return 0;
}