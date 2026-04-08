#include <cstdio>
constexpr int N = 1000005;
int a[N];
int main() {
    // freopen("data/down/eat3.in", "r", stdin);
    // freopen("data/dat.put", "w", stdout);
    int C, n, m, p;
    scanf("%d%d%d", &C, &n, &m);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    int l = 1, r = n;
#define calc(pos) (1ll * a[pos] * a[l + (pos - l + p) % (r - l + 1)])
    for (long long x; m--; ) {
        scanf("%d%lld", &p, &x);
        int tl = l, tr = r;
        // for (int i = l; i <= r; i++)
        //     printf("%lld%c", calc(i), i==r?'\n':' ');
        fflush(stdout);
        while (tl < tr) {
            if (calc(tl) == x) break;
            if (calc(tr) == x) break;
            ++tl, --tr;
        }
        if (calc(tl) == x) printf("L %d\n", tl - l + 1), l = tl+1;
        else printf("R %d\n", r - tr + 1), r = tr-1;
    }
    return 0;
}