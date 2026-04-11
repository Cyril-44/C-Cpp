#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int MAXN = 3010;
ll fact[MAXN], invfact[MAXN];
ll mod;

ll powmod(ll a, ll b) {
  ll res = 1;
  while (b) {
    if (b & 1) res = res * a % mod;
    a = a * a % mod;
    b >>= 1;
  }
  return res;
}

void pre(int n) {
  fact[0] = 1;
  for (int i = 1; i <= n; i++) fact[i] = fact[i - 1] * i % mod;
  invfact[n] = powmod(fact[n], mod - 2);
  for (int i = n - 1; i >= 0; i--) invfact[i] = invfact[i + 1] * (i + 1) % mod;
}

ll C(int n, int k) {
  if (k < 0 || k > n) return 0;
  return fact[n] * invfact[k] % mod * invfact[n - k] % mod;
}

int main() {
  int n;
  cin >> n >> mod;
  pre(n);
  ll ans = 0;
  for (int k = 1; k <= n; k++) {
    ll num = 0;
    for (int r = 1;; r++) {
      int nn = n - r * (k - 1);
      if (nn < r || nn < 0) break;
      ll c = C(nn, r);
      ll f = fact[nn];
      ll sign = (r & 1) ? 1 : (mod - 1);
      num = (num + sign * c % mod * f % mod) % mod;
    }
    ans = (ans + num) % mod;
  }
  cout << ans << endl;
  return 0;
}