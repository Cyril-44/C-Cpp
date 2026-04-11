#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

const ll MOD = 1000000009LL;

int main() {
  int tid, T;
  cin >> tid >> T;
  vector<ll> pw(200010, 1);
  for (int i = 1; i < 200010; i++) {
    pw[i] = pw[i - 1] * 154LL % MOD;
  }
  for (int t = 0; t < T; t++) {
    int n, m;
    cin >> n >> m;
    vector<ll> a(n + 1);
    for (int i = 1; i <= n; i++) cin >> a[i];
    vector<bool> forbidden(n + 1, false);
    for (int i = 0; i < m; i++) {
      int b; cin >> b;
      if (b >= 1 && b <= n) forbidden[b] = true;
    }
    vector<ll> minv(n + 1, LLONG_MAX / 2);
    vector<int> best_p(n + 1, 0);
    vector<int> max_c(n + 1, 0);
    minv[0] = LLONG_MAX / 2;
    for (int j = 1; j <= n; j++) {
      bool cand = (j == 1) || !forbidden[j];
      ll cur = cand ? a[j] : LLONG_MAX / 2;
      minv[j] = min(minv[j - 1], cur);
      if (cand && a[j] == minv[j]) {
        best_p[j] = j;
      } else {
        best_p[j] = best_p[j - 1];
      }
      max_c[j] = cand ? j : max_c[j - 1];
    }
    vector<ll> ans(n + 1);
    auto fill_func = [&](auto self, int L, int R, int m, bool is_type0, bool do_rev) -> void {
      int k = R - L + 1;
      if (k == 0) return;
      if (k == 1) {
        ans[L] = a[1];
        return;
      }
      int p = is_type0 ? best_p[m] : max_c[m];
      int m_inner = p - 1;
      bool inner_type0 = !is_type0;
      int len_left = p;
      int len_suf = k - p;
      if (!do_rev) {
        self(self, L, L + len_left - 1, m_inner, inner_type0, true);
        for (int j = 0; j < len_suf; j++) {
          ans[L + p + j] = a[p + 1 + j];
        }
      } else {
        for (int j = 0; j < len_suf; j++) {
          ans[L + j] = a[k - j];
        }
        self(self, L + len_suf, R, m_inner, inner_type0, false);
      }
    };
    fill_func(fill_func, 1, n, n, true, false);
    ll h = 0;
    for (int i = 1; i <= n; i++) {
      h = (h + pw[i - 1] * ans[i] % MOD) % MOD;
    }
    cout << h << endl;
  }
  return 0;
}