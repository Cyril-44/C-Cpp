#include <cstdio>
#include <algorithm>
#include <vector>
constexpr int N = 200005;
int n, a[N];
struct BIT {
    std::vector<int> tr;
    inline BIT(int n) : tr(n+1) {}
    inline void upd(int p, int x) {
        for (; p <= n; p += p & -p) tr[p] += x;
    }
    inline int sum(int p) {
        int res = 0;
        for (; p > 0; p -= p & -p) res += tr[p];
        return res;
    }
};
int main() {
    int m;
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    for (int x; m--;) {
        scanf("%d", &x);
        std::reverse(a+1, a+1+x);
        BIT f(n);
        long long ans = 0;
        for (int i = 1; i <= x; i++) {
            ans += f.sum(a[i]);
            f.upd(a[i], 1);
        }
        printf("%lld\n", ans);
    }
    return 0;
}