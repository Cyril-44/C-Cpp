#include <cstdio>
#include <cstring>
constexpr int N = 500004;
int a[N], n;
struct BIT {
    int tr[N];
    void upd(int p, int x) { for (; p <= n; p += p & -p) tr[p] += x; }
    int sum(int p) { int res = 0; for (; p > 0; p -= p & -p) res += tr[p]; return res; }
    void build() { memset(tr, 0, sizeof tr); }
} f;
int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        scanf("%d", &n);
        for (int i = 1, x, y; i <= n; i++)
            scanf("%d%d", &x, &y), a[y] = x;
        f.build();
        bool ans = 0;
        for (int i = 1; i <= n; i++) {
            f.upd(a[i], 1);
            ans ^= i - f.sum(a[i]) & 1;
        }
        printf("%d %d\n", ans ? 2 : 0, (1 << 32 - __builtin_clz(n)) - 1 - (ans ? 0 : 2));
    }
    return 0;
}