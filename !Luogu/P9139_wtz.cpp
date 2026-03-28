#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int T;
  cin >> T;
  for (int cas = 0; cas < T; ++cas) {
    unsigned long long s, t;
    int k;
    cin >> s >> t >> k;
    vector<ll> a(k);
    for (int i = 0; i < k; ++i) cin >> a[i];
    if (s == t) {
      cout << 0 << '\n';
      continue;
    }
    int b = __builtin_popcountll(s & t);
    int so = __builtin_popcountll(s) - b;
    int too = __builtin_popcountll(t) - b;
    ll direct = a[b];
    ll mintwo = 4e18;
    for (int c = 0; c <= b; ++c) {
      ll min1 = 4e18;
      for (int p = 0; p <= so; ++p) {
        int r1 = c + p;
        if (r1 < k) min1 = min(min1, a[r1]);
      }
      ll min2 = 4e18;
      for (int q = 0; q <= too; ++q) {
        int r2 = c + q;
        if (r2 < k) min2 = min(min2, a[r2]);
      }
      if (min1 < 4e18 && min2 < 4e18) {
        mintwo = min(mintwo, min1 + min2);
      }
    }
    ll ans = min(direct, mintwo);
    cout << ans << '\n';
  }
}