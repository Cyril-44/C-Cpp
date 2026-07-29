#include <bits/stdc++.h>
#define ll long long
using namespace std;
const int mod = 998244353;
int tid, n, nn;
ll m, a[5005], b[5005], d[5005], f[5005][5005], ans; // f[i][j]表示前i个数j个数在右边，其余任选的方案数
bool cmp(ll x, ll y) {
    return x % m > y % m;
}
ll P(ll x, int y) {
    ll sum = 1;
    for (ll i = x, j = 1; j <= y; i--, j++) { sum = sum * (i % mod) % mod; }
    return sum;
}
int main() {
    cin >> tid >> n >> m;
    for (int i = 1; i <= n; i++) { scanf("%lld", &a[i]); }
    sort(a + 1, a + n + 1, cmp);
    for (int i = 1; i <= n; i++) {
        b[i] = (a[i] + m - 1) / m;
        d[i] = (a[i] % m ? m - a[i] % m : 0);
    }
    f[0][0] = 1;
    for (int i = 1; i <= n; i++) {
        for (int j = 0; j <= i; j++) {
            if (j)
                f[i][j] = (f[i - 1][j] * b[i] + max(0ll, f[i - 1][j - 1] * (d[i] - (j - 1)))) % mod;
            else
                f[i][j] = f[i - 1][j] * b[i] % mod;
        }
    }
    for (int i = 0; i <= n; i++) { ans = (ans + f[n][i] * P(m - i, n - i) % mod * (i & 1 ? -1 : 1) + mod) % mod; }
    cout << ans << endl;
    return 0;
}