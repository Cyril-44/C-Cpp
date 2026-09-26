#include <bits/stdc++.h>
#define ll long long
using namespace std;
const int mod = 1e9 + 7;
int t, n, a[605];
ll fac[605], inv[605], ifac[605], f[605][605];
map<int, int> mp;
ll C(int x, int y) {
    if (x < 0 || y < 0 || x < y) return 0;
    return fac[x] * ifac[y] % mod * ifac[x - y] % mod;
}
int main() {
    fac[0] = fac[1] = inv[0] = inv[1] = ifac[0] = ifac[1] = 1;
    for (int i = 2; i <= 600; i++) {
        fac[i] = fac[i - 1] * i % mod;
        inv[i] = (mod - mod / i) * inv[mod % i] % mod;
        ifac[i] = ifac[i - 1] * inv[i] % mod;
    }
    cin >> t;
    while (t--) {
        cin >> n;
        memset(f, 0, sizeof(f));
        mp.clear();
        for (int i = 1; i <= n; i++) {
            int x, al = 1;
            scanf("%d", &x);
            for (int j = 2; j * j <= x; j++) {
                int cnt = 0;
                while (x % j == 0) {
                    cnt++;
                    x /= j;
                }
                if (cnt & 1) al *= j;
            }
            if (x > 1) al *= x;
            mp[al]++;
        }
        n = 0;
        for (auto x : mp) a[++n] = x.second;
        int limj = 0, sumj = 0;
        f[0][0] = 1;
        for (int i = 1; i <= n; i++) {
            for (int j = 0; j <= limj; j++) {
                for (int k = 1; k <= a[i]; k++) {
                    for (int l = 0; l <= j && l <= k; l++) {
                        f[i][j - l + a[i] - k] =
                            (f[i][j - l + a[i] - k] +
                             f[i - 1][j] * C(j, l) % mod * C(sumj + 1 - j, k - l) % mod * C(a[i] - 1, k - 1)) %
                            mod;
                    }
                }
            }
            limj += a[i] - 1, sumj += a[i];
            for (int j = 0; j <= limj; j++) { f[i][j] = f[i][j] * fac[a[i]] % mod; }
        }
        printf("%lld\n", f[n][0]);
    }
    return 0;
}