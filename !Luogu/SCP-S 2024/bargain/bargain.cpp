#include <stdio.h>
#include <string.h>
#include <algorithm>
const int N = 100005;
char s[N];
struct Num {
    int id, val;
    inline bool operator<(const Num &x) const {
        return val < x.val || val == x.val && id > x.id;
    }
} v[10];
inline int dfs(int n) {
    if (!n) return 0;
    int a[9] = {1, n%10};
    int res = n, tpn;
    while (n/=10) a[++a[0]] = n%10;
    for (int i = 1; i <= a[0]; i++) {
        tpn = 0;
        for (int j = a[0]; j > 0; j--)
            if (j ^ i) tpn = (tpn<<1) + (tpn<<3) + a[j];
        res = std::min(res, v[a[i]-1].val + dfs(tpn));
    }
    return res;
}
int main() {
#ifndef ONLINE_JUDGE
    freopen("bargain.in", "r", stdin);
    freopen("bargain.out", "w", stdout);
#endif
    int t, n, ans, cnt;
    char c;
    scanf("%*d %d", &t);
    while (t--) {
        scanf("%s", s);
        n = strlen(s);
        auto calc = [n] () {
            int res = 0;
            for (int i = 0; i < n; i++)
                if (s[i]) res = (res << 3) + (res << 1) + (s[i] ^ '0');
            return res;
        };
        int h[10] = {};
        Num a[10] = {};
        for (int i = 0; i < n; i++)
            ++h[s[i] ^ '0'];
        for (int i = 0; i < 9; i++) {
            a[i].id = i+1;
            scanf("%d", &a[i].val);
        }
        memcpy(v, a, sizeof v);
        std::sort(a, a + 9);
        ans = 0, cnt = n;
        bool flg = false;
        for (int i = 0; i < 9; i++) {
            if (flg) break;
            for (int j = 0; j < n; j++)
                if (!(s[j] ^ a[i].id ^ '0')) {
                    if (cnt <= 8) {
                        /* int tp = calc();
                        s[j] = 0;
                        int tp2 = calc();
                        if (tp < a[i].val + tp2) {
                            flg = true;
                            ans += tp;
                            break;
                        } */
                        ans += dfs(calc());
                        flg = true;
                        break;
                    }
                    s[j] = 0; --cnt;
                    ans += a[i].val;
                }
        }
        printf("%d\n", ans);
    }
    return 0;
}