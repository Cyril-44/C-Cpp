#include <bits/stdc++.h>
#define ll long long
using namespace std;
const int mod = 998244353;
int t, n, m;
ll ans;
ll power(ll x, ll k) {
    ll cnt = 1;
    for (int i = k; i; i >>= 1) {
        if (i & 1) cnt = cnt * x % mod;
        x = x * x % mod;
    }
    return cnt;
}
int main() {
    cin >> t;
    while (t--) {
        scanf("%d%d", &n, &m);
        ans = n * power(m, n) % mod;
        for (int i = 1; i < m; i++) {
            ll mx = m * power(i, mod - 2) % mod;
            ans = (ans + power(i, n) * (power(mx, n) + mod - 1) % mod * power((mx + mod - 1) % mod, mod - 2)) % mod;
        }
        printf("%lld\n", ans);
    }
    return 0;
}