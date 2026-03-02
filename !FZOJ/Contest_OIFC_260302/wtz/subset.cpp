#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
#define int long long
const int N = (1 << 20) + 10;
int n, k, a[N], b[N], type[N];
int ans[25], len;
unordered_map<int, int> mp;
bool solve(int n, int *a) {
    len = 0;
    while (n > 1) {
        int dis = a[2] - a[1];
        mp.clear();
        memset(type, 0, sizeof(int) * (n + 1));
        for (int i = 1; i <= n; i++) {
            if (mp[a[i]]) {
                type[i] = 2;
                mp[a[i]]--;
            } else {
                type[i] = 1;
                mp[a[i] + dis]++;
            }
        }
        int use = 0, m = 0;
        for (int i = n; i >= 1; i--)
            if (type[i] == 1 && mp[a[i] + dis]) {
                type[i] = 0;
                mp[a[i] + dis]--;
            }
        for (int i = 1; i <= n; i++)
            if (a[i] == 0 && type[i]) use = type[i];
        if (use == 1)
            ans[++len] = dis;
        else
            ans[++len] = -dis;
        for (int i = 1; i <= n; i++)
            if (type[i] == use) a[++m] = a[i];
        if (m * 2 != n) return 0;
        n = m;
    }
    return 1;
}
void work() {
    scanf("%lld%lld", &n, &k);
    n = (1 << n) - k;
    bool zero = 0;
    for (int i = 1; i <= n; i++) {
        scanf("%lld", &a[i]);
        if (!a[i]) zero = 1;
    }
    if (n == 1 && k == 1 && zero) {
        printf("0\n");
        return;
    }
    if (!zero) {
        a[++n] = 0;
        k--;
    }
    sort(a + 1, a + n + 1);
    for (int i = 1; i <= n; i++) b[i] = a[i];
    if (!k) {
        solve(n, b);
        for (int i = 1; i <= len; i++) printf("%lld ", ans[i]);
        putchar('\n');
    } else {
        ll sum = 0, mul = (n + 1) / 2, tmp;
        for (int i = 1; i <= n; i++) sum += a[i];
        tmp = mul * (a[n] + a[1]) - sum;
        if (tmp >= -100000000 && tmp <= 100000000) {
            a[n + 1] = tmp;
            for (int i = 1; i <= n + 1; i++) b[i] = a[i];
            sort(b + 1, b + n + 2);
            if (solve(n + 1, b)) {
                for (int i = 1; i <= len; i++) printf("%lld ", ans[i]);
                putchar('\n');
                return;
            }
        }
        tmp = mul * (a[n] + a[2]) - sum;
        if (tmp >= -100000000 && tmp <= 100000000) {
            a[n + 1] = tmp;
            for (int i = 1; i <= n + 1; i++) b[i] = a[i];
            sort(b + 1, b + n + 2);
            if (solve(n + 1, b)) {
                for (int i = 1; i <= len; i++) printf("%lld ", ans[i]);
                putchar('\n');
                return;
            }
        }
        tmp = mul * (a[n - 1] + a[1]) - sum;
        if (tmp >= -100000000 && tmp <= 100000000) {
            a[n + 1] = tmp;
            for (int i = 1; i <= n + 1; i++) b[i] = a[i];
            sort(b + 1, b + n + 2);
            if (solve(n + 1, b)) {
                for (int i = 1; i <= len; i++) printf("%lld ", ans[i]);
                putchar('\n');
                return;
            }
        }
    }
    return;
}
signed main() {
    freopen("subset.in", "r", stdin);
    freopen("subset.out", "w", stdout);
    int c, t;
    scanf("%lld%lld", &c, &t);
    while (t--) work();
}
/*
2 3 3 -4
2 2 2 2 2 2 2 2
3 3 3 3         3 3 3 3
3 3     3 3     3 3     3 3
-4  -4  -4  -4  -4  -4  -4  -4
4 8 1 5 1 5 -22 2 6 -13 -13 -40
*/