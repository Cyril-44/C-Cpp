#include <bits/stdc++.h>
using namespace std;
using int64 = long long;
static constexpr int MOD = 1000000007;
static constexpr int MAXN = 600;
static constexpr int MAXA = 31623;
int square_free_part(int x, const vector<int> &primes) {
    int res = 1;
    for (int p : primes) {
        if (1LL * p * p > x) break;
        if (x % p == 0) {
            int cnt = 0;
            while (x % p == 0) {
                x /= p;
                cnt ^= 1;
            }
            if (cnt) res *= p;
        }
    }
    if (x > 1) res *= x;
    return res;
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    vector<bool> is_prime(MAXA + 1, true);
    is_prime[0] = is_prime[1] = false;
    vector<int> primes;
    for (int i = 2; i <= MAXA; ++i) {
        if (is_prime[i]) {
            primes.push_back(i);
            if (1LL * i * i <= MAXA) {
                for (int j = i * i; j <= MAXA; j += i) { is_prime[j] = false; }
            }
        }
    }
    int T;
    cin >> T;
    vector<int64> fac(MAXN + 1), ifac(MAXN + 1);
    fac[0] = 1;
    for (int i = 1; i <= MAXN; ++i) { fac[i] = fac[i - 1] * i % MOD; }
    auto qpow = [&](int64 a, int64 b) {
        int64 r = 1;
        while (b) {
            if (b & 1) r = r * a % MOD;
            a = a * a % MOD;
            b >>= 1;
        }
        return r;
    };
    ifac[MAXN] = qpow(fac[MAXN], MOD - 2);
    for (int i = MAXN; i >= 1; --i) { ifac[i - 1] = ifac[i] * i % MOD; }
    auto C = [&](int n, int k) -> int64 {
        if (k < 0 || k > n) return 0;
        return fac[n] * ifac[k] % MOD * ifac[n - k] % MOD;
    };
    while (T--) {
        int n;
        cin >> n;
        vector<int> a(n);
        for (int &x : a) { cin >> x; }
        vector<int> color(n);
        for (int i = 0; i < n; ++i) { color[i] = square_free_part(a[i], primes); }
        sort(color.begin(), color.end());
        vector<int> cnt;
        for (int i = 0; i < n;) {
            int j = i;
            while (j < n && color[j] == color[i]) { ++j; }
            cnt.push_back(j - i);
            i = j;
        }
        vector<int64> dp(n + 1, 0);
        dp[0] = 1;
        int processed = 0;
        for (int c : cnt) {
            vector<int64> g(c + 1, 0);
            for (int j = 1; j <= c; ++j) {
                int64 ways = fac[c] * C(c - 1, j - 1) % MOD * ifac[j] % MOD;
                if ((c - j) & 1) { ways = (MOD - ways) % MOD; }
                g[j] = ways;
            }
            vector<int64> ndp(n + 1, 0);
            for (int i = 0; i <= processed; ++i) {
                if (dp[i] == 0) continue;
                for (int j = 1; j <= c; ++j) {
                    ndp[i + j] += dp[i] * g[j] % MOD;
                    if (ndp[i + j] >= MOD) { ndp[i + j] -= MOD; }
                }
            }
            dp.swap(ndp);
            processed += c;
        }
        int64 ans = 0;
        for (int t = 1; t <= n; ++t) {
            ans += dp[t] * fac[t] % MOD;
            if (ans >= MOD) { ans -= MOD; }
        }
        cout << ans << '\n';
    }
    return 0;
}