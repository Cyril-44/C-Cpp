#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll MOD = 998244353;

ll modpow(ll b, ll e) {
  ll res = 1;
  while (e) {
    if (e & 1) res = res * b % MOD;
    b = b * b % MOD;
    e >>= 1;
  }
  return res;
}
ll modinv(ll x) { return modpow(x, MOD - 2); }

using vvll = vector<vector<ll>>;
using vec = vector<ll>;

vvll matmul(const vvll& a, const vvll& b) {
  vvll c(3, vector<ll>(3, 0));
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      for (int k = 0; k < 3; k++)
        c[i][j] = (c[i][j] + a[i][k] * b[k][j] % MOD) % MOD;
  return c;
}

vvll matpow(vvll a, ll e) {
  vvll res(3, vector<ll>(3, 0));
  for (int i = 0; i < 3; i++) res[i][i] = 1;
  while (e > 0) {
    if (e & 1) res = matmul(res, a);
    a = matmul(a, a);
    e >>= 1;
  }
  return res;
}

vec mat_vec_mul(const vvll& a, const vec& v) {
  vec res(3, 0);
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      res[i] = (res[i] + a[i][j] * v[j] % MOD) % MOD;
  return res;
}

struct SegTree {
  int n;
  vector<ll> sum, lmul, ladd;
  vector<bool> hasl;
  SegTree(int _n) : n(_n), sum(4 * (_n + 5), 0LL), lmul(4 * (_n + 5), 1LL), ladd(4 * (_n + 5), 0LL), hasl(4 * (_n + 5), false) {}
  
  void push(int id, int l, int r) {
    if (hasl[id]) {
      if (l != r) {
        int mid = (l + r) / 2;
        // left
        sum[2 * id] = (lmul[id] * sum[2 * id] % MOD + ladd[id] * (mid - l + 1LL) % MOD) % MOD;
        if (hasl[2 * id]) {
          lmul[2 * id] = lmul[id] * lmul[2 * id] % MOD;
          ladd[2 * id] = (lmul[id] * ladd[2 * id] + ladd[id]) % MOD;
        } else {
          lmul[2 * id] = lmul[id];
          ladd[2 * id] = ladd[id];
          hasl[2 * id] = true;
        }
        // right
        sum[2 * id + 1] = (lmul[id] * sum[2 * id + 1] % MOD + ladd[id] * (r - mid) % MOD) % MOD;
        if (hasl[2 * id + 1]) {
          lmul[2 * id + 1] = lmul[id] * lmul[2 * id + 1] % MOD;
          ladd[2 * id + 1] = (lmul[id] * ladd[2 * id + 1] + ladd[id]) % MOD;
        } else {
          lmul[2 * id + 1] = lmul[id];
          ladd[2 * id + 1] = ladd[id];
          hasl[2 * id + 1] = true;
        }
      }
      hasl[id] = false;
      lmul[id] = 1;
      ladd[id] = 0;
    }
  }
  
  void update(int id, int l, int r, int ql, int qr, ll mulv, ll addv) {
    push(id, l, r);
    if (qr < l || ql > r) return;
    if (ql <= l && r <= qr) {
      sum[id] = (mulv * sum[id] % MOD + addv * (r - l + 1LL) % MOD) % MOD;
      if (l != r) {
        lmul[id] = mulv;
        ladd[id] = addv;
        hasl[id] = true;
      }
      return;
    }
    int mid = (l + r) / 2;
    update(2 * id, l, mid, ql, qr, mulv, addv);
    update(2 * id + 1, mid + 1, r, ql, qr, mulv, addv);
    sum[id] = (sum[2 * id] + sum[2 * id + 1]) % MOD;
  }
  
  ll query(int id, int l, int r, int ql, int qr) {
    push(id, l, r);
    if (qr < l || ql > r) return 0;
    if (ql <= l && r <= qr) return sum[id];
    int mid = (l + r) / 2;
    ll p1 = query(2 * id, l, mid, ql, qr);
    ll p2 = query(2 * id + 1, mid + 1, r, ql, qr);
    return (p1 + p2) % MOD;
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  freopen("magic.in", "r", stdin);
  freopen("magic.out", "w", stdout);
  int tid;
  cin >> tid;
  int N, M;
  cin >> N >> M;
  SegTree st(M);
  ll cur_ef = 1;
  for (int i = 0; i < N; i++) {
    ll L, R, C;
    cin >> L >> R >> C;
    ll k = R - L + 1;
    ll ik = modinv(k);
    ll d = ((k - 1 + MOD) % MOD) * ik % MOD;
    ll sum0v = st.query(1, 1, M, L, R);
    ll avg0v = sum0v * ik % MOD;
    vvll AA(3, vector<ll>(3, 0LL));
    AA[0][0] = 2;
    AA[0][1] = (MOD - 1) % MOD;
    AA[0][2] = (MOD - avg0v) % MOD;
    AA[1][0] = ik;
    AA[1][1] = d;
    AA[1][2] = 0;
    AA[2][0] = 0;
    AA[2][1] = 0;
    AA[2][2] = d;
    vec U0v = {cur_ef, 0LL, 1LL};
    if (C > 0) {
      vvll Ap = matpow(AA, C);
      vec Uc = mat_vec_mul(Ap, U0v);
      cur_ef = Uc[0];
      ll Mff = Uc[2];
      ll Cff = Uc[1];
      st.update(1, 1, M, L, R, Mff, Cff);
    }
  }
  cout << cur_ef << endl;
}