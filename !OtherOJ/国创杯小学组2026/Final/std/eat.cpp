#include <cstdio>
constexpr int N = 1000005;
int a[N];
int main() {
    freopen("eat.in", "r", stdin);
    freopen("eat.out", "w", stdout);
    int C, n, m, p;
    scanf("%d%d%d", &C, &n, &m);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    int l = 1, r = n;
#define calc(pos) (1ll * a[pos] * a[l + (pos - l + p) % (r - l + 1)])
    for (long long x; m--; ) {
        scanf("%d%lld", &p, &x);
        int tl = l, tr = r;
        if (tl == tr) { puts("F"); break; }
        while (tl <= tr) {
            if (calc(tl) == x) break;
            if (calc(tr) == x) break;
            ++tl, --tr;
        }
        if (calc(tl) == x) printf("L %d\n", tl - l + 1), l = tl+1;
        else if (calc(tr) == x) printf("R %d\n", r - tr + 1), r = tr-1;
        else puts("F");
    }
    return 0;
}