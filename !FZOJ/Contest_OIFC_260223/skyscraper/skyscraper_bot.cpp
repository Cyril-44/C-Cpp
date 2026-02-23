#include <bits/stdc++.h>
using namespace std;

constexpr int MOD = 998244353, G = 3;

long long modpow(long long a, long long e) {
    long long r = 1 % MOD;
    a %= MOD;
    while (e > 0) {
        if (e & 1) r = r * a % MOD;
        a = a * a % MOD;
        e >>= 1;
    }
    return r;
}
long long modinv(long long a) { return modpow(a, MOD - 2); }

void ntt(vector<long long>& a, bool invert) {
    int n = (int)a.size();
    static vector<int> rev;
    static vector<long long> roots{0, 1};

    if ((int)rev.size() != n) {
        int k = __builtin_ctz(n);
        rev.assign(n, 0);
        for (int i = 0; i < n; i++) {
            rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (k - 1));
        }
    }
    if ((int)roots.size() < n) {
        int k = __builtin_ctz(roots.size());
        roots.resize(n);
        while ((1 << k) < n) {
            long long e = modpow(G, (MOD - 1) >> (k + 1));
            for (int i = 1 << (k - 1); i < (1 << k); i++) {
                roots[2 * i] = roots[i];
                roots[2 * i + 1] = roots[i] * e % MOD;
            }
            k++;
        }
    }

    for (int i = 0; i < n; i++) {
        if (i < rev[i]) swap(a[i], a[rev[i]]);
    }

    for (int len = 1; len < n; len <<= 1) {
        for (int i = 0; i < n; i += 2 * len) {
            for (int j = 0; j < len; j++) {
                long long u = a[i + j];
                long long v = a[i + j + len] * roots[len + j] % MOD;
                a[i + j] = u + v;
                if (a[i + j] >= MOD) a[i + j] -= MOD;
                a[i + j + len] = u - v;
                if (a[i + j + len] < 0) a[i + j + len] += MOD;
            }
        }
    }

    if (invert) {
        reverse(a.begin() + 1, a.end());
        long long inv_n = modinv(n);
        for (int i = 0; i < n; i++) a[i] = a[i] * inv_n % MOD;
    }
}

static inline long long pow(long long x, int c) {
    if (c == 1) return x;
    if (c == 2) return x * x % MOD;
    if (c == 3) return x * x % MOD * x % MOD;
    if (c == 4) { long long t = x * x % MOD; return t * t % MOD; }
    long long t = x * x % MOD;
    t = t * t % MOD;
    return t * x % MOD;
}

int main() {
    freopen("skyscraper.in", "r", stdin);
    freopen("skyscraper.out", "w", stdout);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m, c;
    long long v;
    cin >> n >> m >> c >> v;
    int M = 1 << m;

    long long root = modpow(G, (MOD - 1) / M);

    vector<long long> S(M);

    auto geom = [&](long long r)->long long {
        if (r == 1) return v % MOD;
        long long rv = modpow(r, v);
        long long num = (1 - rv) % MOD; if (num < 0) num += MOD;
        long long den = (1 - r) % MOD;  if (den < 0) den += MOD;
        return r * num % MOD * modinv(den) % MOD;
    };

    for (int j = 0; j < M; j++) {
        long long omega = modpow(root, j);

        // x0 = f^{(n-2)}(1)
        long long x0 = 1;
        for (int step = 0; step < n - 2; step++) {
            long long t = (1 + omega * x0) % MOD;
            x0 = pow(t, c);
        }
        // x1 = f^{(n-1)}(1)
        long long t1 = (1 + omega * x0) % MOD;
        long long x1 = pow(t1, c);

        long long val = (geom(omega * x1 % MOD) - geom(omega * x0 % MOD)) % MOD;
        if (val < 0) val += MOD;
        S[j] = val;
    }

    ntt(S, true);

    for (int k = 0; k < M; k++) {
        cout << S[k] << (k + 1 == M ? '\n' : ' ');
    }
    return 0;
}
