#include <bits/stdc++.h>
#define ll long long
using namespace std;
const int mod = 998244353;
int n, m, k, a[200005];
ll ans, fac[20000005], inv[20000005], ifac[20000005];
ll C(int x, int y) {
    if (x < 0 || y < 0 || x < y) return 0;
    return fac[x] * ifac[y] % mod * ifac[x - y] % mod;
}
ll calc(int x, int y) {
    return C(x + y - 1, x - 1);
}
int main() {
    fac[0] = fac[1] = inv[0] = inv[1] = ifac[0] = ifac[1] = 1;
    for (int i = 2; i <= 20000000; i++) {
        fac[i] = fac[i - 1] * i % mod;
        inv[i] = (mod - mod / i) * inv[mod % i] % mod;
        ifac[i] = ifac[i - 1] * inv[i] % mod;
    }
    cin >> n >> m >> k;
    for (int i = 1; i <= k; i++) { scanf("%d", &a[i]); }
    sort(a + 1, a + k + 1);
    a[k + 1] = m + 1;
    ans = calc(m, n - k);
    for (int i = 1; i <= k + 1; i++) {
        if (a[i] > a[i - 1] + 1) {
            int x = a[i] - a[i - 1] - 1, dead = (n - k + i - 1) / 2;
            for (int y = dead + 1; y <= n - k; y++) {
                ans = (ans - calc(x, y) * calc(m - x, n - k - y) % mod + mod) % mod;
            }
        }
    }
    cout << ans << endl;
    return 0;
}
/*
每个点一个deadline=(n-k+i-1)/2
S(n,m)=C(n+m-1,n-1)将n个数填到m个位置中
考虑容斥，答案=S(m,n-k)-\sum_y S(x,y)*S(m-x,n-k-y)

难点在于需要枚举y,即超限的数的个数(n)
S(x,y)*S(m-x,n-k-y)=C(x+y-1,x-1)*C(m+n-k-x-y-1,m-x-1)=

fac[x+y-1]*ifac[n-k-y]*ifac[y]*fac[m+n-k-x-y-1]*ifac[x-1]*ifac[m-x-1]

\sum fac[x+y-1]*fac[m+n-k-x-y-1] 满足两数和=m+n-k-2 ?<=x+y<=?
*/