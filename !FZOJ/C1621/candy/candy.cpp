#include <bits/stdc++.h>
constexpr int N = 200005;
char s[N];
int n;
struct BIT {
    int tr[N << 1];
    void upd(int p, int x) {
        for (p = (n+1) - p; p <= n * 2 + 1; p += p & -p) tr[p] += x;
    }
    int sum(int p) const {
        int res = 0;
        for (p = (n+1) - p; p > 0; p -= p & -p) res += tr[p];
        return res;
    }
    void clear() { memset(tr, 0, sizeof(int) * (2*n+2)); }
} f[3];
int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        scanf("%d %s", &n, s+1);
        for (int x:{0,1,2}) f[x].clear();
        int now = 0;
        int64_t ans = 0;
        f[0].upd(0, 1);
        for (int i = 1; i <= n; i++) {
            now += s[i] == '+' ? 1 : -1;
            int ctg = ((now % 3) + 3) % 3;
            ans += f[ctg].sum(now);
            f[ctg].upd(now, 1);
        }
        printf("%lld\n", ans);
    }
}