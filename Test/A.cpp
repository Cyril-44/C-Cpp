#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
template <typename T> void in(T &x) {
    x = 0;
    char c = getchar();
    int f = 1;
    while (c < '0' || c > '9') {
        if (c == '-')
            f = -1;
        c = getchar();
    }
    while (c >= '0' && c <= '9') {
        x = x * 10 + c - '0';
        c = getchar();
    }
    x *= f;
}
const int Mod = 998244353;
ll n;
int m, k;
ll ksm(ll a, ll b) {
    ll res = 1;
    while (b) {
        if (b & 1)
            res = res * a % Mod;
        a = a * a % Mod;
        b >>= 1;
    }
    return res;
}
const int N = 10010000;
ll fac[N + 10], ny[N + 10];
ll C(ll a, int b) {
    if (a < 0 || b > a)
        return 0;
    if (a <= 10010000)
        return fac[a] * ny[b] % Mod * ny[a - b] % Mod;
    a %= Mod;
    ll res = 1;
    for (int i = 0; i < b; i++)
        res = res * (a + Mod - i) % Mod;
    res = res * ny[b] % Mod;
    return res;
}
int main() {
    in(n);
    in(m);
    in(k);
    fac[0] = 1;
    for (int i = 1; i <= N; i++)
        fac[i] = fac[i - 1] * i % Mod;
    ny[N] = ksm(fac[N], Mod - 2);
    for (int i = N - 1; i >= 0; i--)
        ny[i] = ny[i + 1] * (i + 1) % Mod;
    //    for(int i=0;i<=10;i++){
    //        printf("%lld %lld\n",fac[i],ny[i]);
    //    }
    ll ans = 0;
    for (int i = n % m; i <= n % m + (k - 1) * m && i <= n; i += m) {
        ll val = 0;
        for (int j = 0; j <= k; j++) {
            if (j & 1)
                val = (val + Mod -
                       C(i - 1 - j * (m - 1), k - 1) * C(k, j) % Mod) %
                      Mod;
            else
                val = (val + C(i - 1 - j * (m - 1), k - 1) * C(k, j)) % Mod;
        }
        //        printf("%lld %d\n",(n-i)/m+k-1,k-1);
        (ans += val * C((n - i) / m + k - 1, k - 1)) %= Mod;
    }
    printf("%lld", ans);
    return 0;
}