#include <bits/stdc++.h>
using namespace std;
using ll = long long;

map<ll, ll> mf;

inline int mod4_ll(ll x) {
  int m = int(x % 4);
  if (m < 0) m += 4;
  return m;
}

ll cum(ll x) {
  if (x < 0) return 0;
  ll m = x % 4;
  if (m == 0) return x;
  if (m == 1) return 1;
  if (m == 2) return x + 1;
  return 0;
}

ll geth(ll n) {
  if (n < 0) return 0;
  ll a = n / 4 + 1;
  ll b = (n >= 2 ? (n - 2) / 4 + 1 : 0);
  ll c = (n >= 1 ? 1 : 0);
  return a + b + c;
}

ll geto(ll n);

ll getf(ll n) {
  if (n < 0) return 0;
  if (n == 0) return 1;
  if (mf.count(n)) return mf[n];
  ll p = 1LL << (63 - __builtin_clzll(n));
  ll mm = n - p;
  ll hh = geth(p - 1);
  ll oo = geto(mm);
  ll ii = geth(mm);
  return mf[n] = p + hh + oo - ii;
}

ll geto(ll n) {
  if (n < 0) return 0;
  if (n == 0) return 1;
  ll ff = getf(n);
  return ff - (n % 4 == 0);
}

ll brute(ll A, ll B) {
  set<ll> st;
  ll start = max(0LL, A);
  if (start <= B) {
    for (ll x = start; x <= B; ++x) {
      ll cx = cum(x);
      for (ll y = start; y < x; ++y) {
        ll cy = cum(y);
        st.insert(cx ^ cy);
      }
    }
  }
  if (A < 0) {
    for (ll x = 0; x <= B; ++x) {
      st.insert(cum(x) ^ 0LL);
    }
  }
  return st.size();
}

ll union_size(vector<pair<ll, ll>> ints, ll c) {
  if (ints.empty()) return 0;
  sort(ints.begin(), ints.end());
  vector<pair<ll, ll>> merged;
  ll cs = ints[0].first, ce = ints[0].second;
  for (size_t i = 1; i < ints.size(); ++i) {
    ll ns = ints[i].first, ne = ints[i].second;
    if (ns <= ce + 4) {
      ce = max(ce, ne);
    } else {
      merged.emplace_back(cs, ce);
      cs = ns;
      ce = ne;
    }
  }
  merged.emplace_back(cs, ce);
  ll tot = 0;
  for (auto [s, e] : merged) {
    if (s > e) continue;
    int sm = mod4_ll(s);
    int rem = (int(c) - sm + 4) % 4;
    ll first = s + rem;
    if (first > e) continue;
    tot += ((e - first) / 4) + 1;
  }
  return tot;
}

ll solve(ll l, ll r) {
  if (l > r) return 0;
  if (l == r) return 1;
  ll A = l - 1;
  ll B = r;
  ll leng = B - A + 1;
  if (l == 0) return getf(B);
  if (leng <= 4000) return brute(A, B);

  // helper mod
  auto mA = mod4_ll(A);
  auto mB = mod4_ll(B);

  // AP0: values v ≡ 0 (mod 4) in [A, B]
  ll v_min0 = A + ((4 - mA) % 4); // first >= A with mod 0
  ll num0 = 0;
  ll k_low0 = 0, k_high0 = 0;
  ll v_max0 = 0;
  if (v_min0 <= B) {
    v_max0 = B - mB; // last <= B with mod 0
    num0 = (v_max0 - v_min0) / 4 + 1;
    k_low0 = v_min0 / 4;
    k_high0 = v_max0 / 4;
  }

  // AP3: indices i ≡ 2 (mod 4) in [A, B], then v = i+1 ≡ 3 (mod 4)
  ll i_min2 = A + ((2 - mA + 4) % 4); // first i >= A with i%4==2
  ll num3 = 0;
  ll k_low3 = 0, k_high3 = 0;
  ll v_min3 = 0, v_max3 = 0;
  if (i_min2 <= B) {
    int mB_i = mB;
    ll i_max2 = B - ((mB_i - 2 + 4) % 4); // last i <= B with i%4==2
    num3 = (i_max2 - i_min2) / 4 + 1;
    v_min3 = i_min2 + 1;
    v_max3 = i_max2 + 1;
    k_low3 = (i_min2 - 2) / 4;
    k_high3 = (i_max2 - 2) / 4;
  }

  // compute max z values
  ll max_z00 = 0;
  if (num0 >= 1) max_z00 = k_low0 ^ k_high0;
  ll max_z33 = 0;
  if (num3 >= 1) max_z33 = k_low3 ^ k_high3;
  ll max_z03 = 0;
  if (num0 >= 1 && num3 >= 1) {
    max_z03 = max({k_low0 ^ k_low3, k_low0 ^ k_high3, k_high0 ^ k_low3, k_high0 ^ k_high3});
  }

  // small mod0 intervals
  vector<pair<ll, ll>> iv0;
  if (num0 >= 1) iv0.emplace_back(0, 4 * max_z00);
  if (num3 >= 1) iv0.emplace_back(0, 4 * max_z33);
  ll total0 = union_size(iv0, 0);

  // small mod3 intervals
  vector<pair<ll, ll>> iv3;
  if (num0 >= 1 && num3 >= 1) iv3.emplace_back(3, 4 * max_z03 + 3);
  ll total3 = 0;
  if (!iv3.empty()) total3 = union_size(iv3, 3);

  // large mod1 (from v ≡ 0 values XOR 1)
  ll size1 = 0;
  if (num0 >= 1) {
    ll low1 = v_min0 ^ 1;
    ll high1 = v_max0 ^ 1;
    if (low1 <= high1) size1 = ((high1 - low1) / 4) + 1;
  }

  // large mod2 (from v ≡ 3 values XOR 1)
  ll size2 = 0;
  if (num3 >= 1) {
    ll low2 = v_min3 ^ 1;
    ll high2 = v_max3 ^ 1;
    if (low2 <= high2) size2 = ((high2 - low2) / 4) + 1;
  }

  // large mod3 (the v ≡ 3 themselves)
  ll total3_large = 0;
  if (num3 >= 1) {
    ll low3l = v_min3;
    ll high3l = v_max3;
    if (low3l <= high3l) total3_large = ((high3l - low3l) / 4) + 1;
  }
  total3 += total3_large;

  // add large mod0 interval to iv0 and recompute union
  if (num0 >= 1) {
    iv0.emplace_back(v_min0, v_max0);
    total0 = union_size(iv0, 0);
  }

  ll ans = total0 + size1 + size2 + total3;
  return ans;
}

int main() {
  freopen("xor.in", "r", stdin);
  freopen("xor.out", "w", stdout);
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int T;
  if (!(cin >> T)) return 0;
  for (int t = 0; t < T; ++t) {
    ll l, r;
    cin >> l >> r;
    cout << solve(l, r) << '\n';
  }
  return 0;
}
