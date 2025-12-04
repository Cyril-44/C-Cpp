#include <stdio.h>
#include <string.h>
#include <algorithm>
constexpr int N = 20005;
struct Cow { int v, x; } a[N];
int n;
struct BIT {
    int tr[N];
    inline void upd(int p, int x) {
        for(; p <= 20000; p += p & -p) tr[p] += x;
    }
    inline int sum(int p) {
        int sum = 0;
        for (; p > 0; p -= p & -p) sum += tr[p];
        return sum;
    }
} f, g;
int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        scanf("%d%d", &a[i].v, &a[i].x);
    std::sort(a+1, a+1 + n, [](const Cow&x, const Cow&y){return x.v < y.v;});
    int sumx = 0;
    long long ans = 0;
    for (int i = 1; i <= n; i++) {
        int cnt = g.sum(a[i].x), sum = f.sum(a[i].x);
        ans += 1ll * a[i].v * (cnt * a[i].x - sum + sumx - sum - (i - 1 - cnt) * a[i].x);
        sumx += a[i].x;
        f.upd(a[i].x, a[i].x);
        g.upd(a[i].x, 1);
    }
    printf("%lld\n", ans);
    return 0;
}