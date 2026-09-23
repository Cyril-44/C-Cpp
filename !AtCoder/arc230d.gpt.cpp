#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;
const int G = 3;

int modpow(long long a, long long e) {
    long long r = 1;
    while (e) {
        if (e & 1) r = r * a % MOD;
        a = a * a % MOD;
        e >>= 1;
    }
    return (int)r;
}

void ntt(vector<int>& a, bool inv) {
    int n = a.size();

    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }

    for (int len = 2; len <= n; len <<= 1) {
        int wlen = modpow(G, (MOD - 1) / len);
        if (inv) wlen = modpow(wlen, MOD - 2);

        for (int i = 0; i < n; i += len) {
            long long w = 1;
            int half = len >> 1;

            for (int j = 0; j < half; ++j) {
                int u = a[i + j];
                int v = (int)(w * a[i + j + half] % MOD);

                int x = u + v;
                if (x >= MOD) x -= MOD;

                int y = u - v;
                if (y < 0) y += MOD;

                a[i + j] = x;
                a[i + j + half] = y;

                w = w * wlen % MOD;
            }
        }
    }

    if (inv) {
        int ni = modpow(n, MOD - 2);
        for (int& x : a)
            x = (long long)x * ni % MOD;
    }
}

vector<int> convolution(vector<int> a, vector<int> b) {
    if (a.empty() || b.empty()) return {};

    int need = a.size() + b.size() - 1;

    if ((long long)a.size() * b.size() <= 4096) {
        vector<int> c(need);
        for (int i = 0; i < (int)a.size(); ++i) {
            for (int j = 0; j < (int)b.size(); ++j) {
                c[i + j] =
                    (c[i + j] + (long long)a[i] * b[j]) % MOD;
            }
        }
        return c;
    }

    int n = 1;
    while (n < need) n <<= 1;

    a.resize(n);
    b.resize(n);

    ntt(a, false);
    ntt(b, false);

    for (int i = 0; i < n; ++i)
        a[i] = (long long)a[i] * b[i] % MOD;

    ntt(a, true);
    a.resize(need);

    return a;
}

vector<int> fps_inverse(const vector<int>& a, int n) {
    vector<int> r(1, modpow(a[0], MOD - 2));

    for (int sz = 1; sz < n; sz <<= 1) {
        int m = min(2 * sz, n);

        vector<int> f(min((int)a.size(), m));
        copy(a.begin(), a.begin() + f.size(), f.begin());

        vector<int> t = convolution(f, r);
        t.resize(m);

        for (int i = 0; i < m; ++i)
            t[i] = t[i] ? MOD - t[i] : 0;

        t[0] += 2;
        if (t[0] >= MOD) t[0] -= MOD;

        r = convolution(r, t);
        r.resize(m);
    }

    r.resize(n);
    return r;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int H, W, M;
    cin >> H >> W >> M;

    int D = H * W;

    vector<int> fac(D + 3), ifac(D + 3), inv(D + 3);

    fac[0] = 1;
    for (int i = 1; i <= D + 2; ++i)
        fac[i] = (long long)fac[i - 1] * i % MOD;

    ifac[D + 2] = modpow(fac[D + 2], MOD - 2);
    for (int i = D + 2; i >= 1; --i)
        ifac[i - 1] = (long long)ifac[i] * i % MOD;

    for (int i = 1; i <= D + 2; ++i)
        inv[i] = (long long)fac[i - 1] * ifac[i] % MOD;

    auto C = [&](int n, int k) {
        return (long long)fac[n] * ifac[k] % MOD * ifac[n - k] % MOD;
    };

    int N = M + 1;

    vector<int> powM(D + 1), powN(D + 2);
    powM[0] = powN[0] = 1;

    for (int i = 1; i <= D; ++i)
        powM[i] = (long long)powM[i - 1] * M % MOD;

    for (int i = 1; i <= D + 1; ++i)
        powN[i] = (long long)powN[i - 1] * N % MOD;

    vector<int> den(D + 1);
    for (int i = 0; i <= D; ++i)
        den[i] = ifac[i + 1];

    vector<int> bern = fps_inverse(den, D + 1);

    vector<int> c(D + 2);
    c[0] = 0;

    for (int i = 1; i <= D + 1; ++i)
        c[i] = (long long)powN[i] * ifac[i] % MOD;

    vector<int> tmp = convolution(bern, c);

    vector<int> power_sum(D + 1);
    power_sum[0] = M;

    for (int d = 1; d <= D; ++d)
        power_sum[d] = (long long)fac[d] * tmp[d + 1] % MOD;

    int invN = modpow(N, MOD - 2);

    vector<int> step(D + 2);
    for (int i = 1; i <= D + 1; ++i)
        step[i] = (long long)inv[i] * invN % MOD;

    long long ans = 0;

    for (int a = 1; a <= H; ++a) {
        for (int b = 1; b <= W; ++b) {
            int p = a * (W - b);
            int q = (H - a) * b;
            int r = (H - a) * (W - b);

            int u = min(p, q);
            int v = max(p, q);

            long long s = 0;
            int coef = powN[u];

            for (int t = 0; t <= u; ++t) {
                s += (long long)coef * power_sum[v + t] % MOD;
                if (s >= MOD) s -= MOD;

                if (t != u) {
                    coef = (long long)coef * (u - t) % MOD
                         * step[t + 1] % MOD;

                    if (coef) coef = MOD - coef;
                }
            }

            long long term =
                s * C(H, a) % MOD
                 * C(W, b) % MOD
                 * powM[r] % MOD;

            if ((a + b) & 1)
                ans -= term;
            else
                ans += term;

            ans %= MOD;
        }
    }

    if (ans < 0) ans += MOD;
    cout << ans << '\n';

    return 0;
}