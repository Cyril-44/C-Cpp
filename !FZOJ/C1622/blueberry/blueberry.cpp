#include <bits/stdc++.h>
#include <cmath>
constexpr int N = 100005;
struct Node {
    int u, v, w;
} a[N];
int n, m;
bool check(int64_t x) {
    int64_t cnt = 0;
    for (int i = 1; i <= n; i++) if (1ll * a[i].w * a[i].u >= x)
        cnt += (1ll * a[i].w * a[i].u - x) / (1ll * a[i].w * a[i].v) + 1;
    return cnt < m;
}
int64_t calc(int64_t x) {
    int64_t sum = 0, res = 0;
    for (int i = 1; i <= n; i++) if (1ll * a[i].w * a[i].u >= x) {
        int64_t cnt = (1ll * a[i].w * a[i].u - x) / (1ll * a[i].w * a[i].v) + 1;
        sum += cnt;
        res += 1ll * a[i].w * (1ll * cnt * a[i].u - cnt * (cnt - 1ll) / 2 * a[i].v);
    }
    res -= x * (sum - m);
    return res;
}
int main() {
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++)
        scanf("%d%d%d", &a[i].u, &a[i].v, &a[i].w);
    int64_t l = 1, r = 1e10;
    while (l <= r) {
        int64_t mid = l + r >> 1;
        if (check(mid)) r = mid - 1;
        else l = mid + 1;
    }
    printf("%d\n", int(calc(r) % 998244353));
}