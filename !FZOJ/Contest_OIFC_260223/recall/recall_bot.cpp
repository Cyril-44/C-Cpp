#include <bits/stdc++.h>
using namespace std;

static const int MOD = 1000000007;

// ---------- mod pow ----------
static long long mod_pow(long long a, long long e, long long mod) {
    long long r = 1 % mod;
    while (e > 0) {
        if (e & 1) r = (__int128)r * a % mod;
        a = (__int128)a * a % mod;
        e >>= 1;
    }
    return r;
}

// ---------- NTT (for one prime) ----------
template<int MODP, int G>
struct NTT {
    static int addmod(int a, int b){ a+=b; if(a>=MODP) a-=MODP; return a; }
    static int submod(int a, int b){ a-=b; if(a<0) a+=MODP; return a; }

    static void ntt(vector<int> & a, bool invert) {
        int n = (int)a.size();
        for (int i=1, j=0; i<n; i++) {
            int bit = n >> 1;
            for (; j & bit; bit >>= 1) j ^= bit;
            j ^= bit;
            if (i < j) swap(a[i], a[j]);
        }
        for (int len=2; len<=n; len<<=1) {
            long long wlen = mod_pow(G, (MODP-1)/len, MODP);
            if (invert) wlen = mod_pow(wlen, MODP-2, MODP);
            for (int i=0; i<n; i+=len) {
                long long w = 1;
                int half = len >> 1;
                for (int j=0; j<half; j++) {
                    int u = a[i+j];
                    int v = (int)((__int128)a[i+j+half] * w % MODP);
                    a[i+j] = addmod(u, v);
                    a[i+j+half] = submod(u, v);
                    w = (__int128)w * wlen % MODP;
                }
            }
        }
        if (invert) {
            long long inv_n = mod_pow(n, MODP-2, MODP);
            for (int & x : a) x = (int)((__int128)x * inv_n % MODP);
        }
    }

    static vector<int> convolution(vector<int> a, vector<int> b) {
        if (a.empty() || b.empty()) return {};
        int n1 = (int)a.size(), n2 = (int)b.size();
        int n = 1;
        while (n < n1 + n2 - 1) n <<= 1;
        a.resize(n); b.resize(n);
        ntt(a, false); ntt(b, false);
        for (int i=0;i<n;i++) a[i] = (int)((__int128)a[i]*b[i] % MODP);
        ntt(a, true);
        a.resize(n1 + n2 - 1);
        return a;
    }
};

// three NTT primes
// 998244353 (g=3), 1004535809 (g=3), 469762049 (g=3)
using NTT1 = NTT<998244353, 3>;
using NTT2 = NTT<1004535809, 3>;
using NTT3 = NTT<469762049, 3>;

// CRT combine to MOD=1e9+7
static vector<int> convolution_mod1e9p7(const vector<int>& A, const vector<int>& B) {
    if (A.empty() || B.empty()) return {};
    vector<int> a1(A.size()), b1(B.size());
    vector<int> a2(A.size()), b2(B.size());
    vector<int> a3(A.size()), b3(B.size());
    for (size_t i=0;i<A.size();i++){
        int x=A[i];
        a1[i]=x%998244353;
        a2[i]=x%1004535809;
        a3[i]=x%469762049;
    }
    for (size_t i=0;i<B.size();i++){
        int x=B[i];
        b1[i]=x%998244353;
        b2[i]=x%1004535809;
        b3[i]=x%469762049;
    }

    auto c1 = NTT1::convolution(a1,b1);
    auto c2 = NTT2::convolution(a2,b2);
    auto c3 = NTT3::convolution(a3,b3);

    int n = (int)c1.size();
    vector<int> res(n);

    const long long m1 = 998244353LL;
    const long long m2 = 1004535809LL;
    const long long m3 = 469762049LL;

    // precompute inverses for CRT
    long long inv_m1_mod_m2 = mod_pow(m1 % m2, m2-2, m2);
    long long inv_m12_mod_m3 = mod_pow((m1*m2)%m3, m3-2, m3);

    for (int i=0;i<n;i++){
        long long x1 = c1[i];
        long long x2 = c2[i];
        long long x3 = c3[i];

        // solve:
        // t1 = x1
        // t2 = x1 + m1 * k2  (mod m2) => k2 = (x2 - x1) * inv(m1) mod m2
        long long k2 = ( (x2 - x1) % m2 + m2 ) % m2;
        k2 = (__int128)k2 * inv_m1_mod_m2 % m2;
        long long t2 = x1 + m1 * k2; // exact under 128-bit

        // t3 = t2 + m1*m2 * k3 (mod m3)
        long long t2_mod_m3 = (long long)(t2 % m3);
        long long k3 = ((x3 - t2_mod_m3) % m3 + m3) % m3;
        k3 = (__int128)k3 * inv_m12_mod_m3 % m3;

        __int128 t3 = (__int128)t2 + (__int128)(m1*m2) * k3; // exact in 128-bit
        long long ans = (long long)(t3 % MOD);
        res[i] = (int)ans;
    }
    return res;
}

// truncate multiplication
static vector<int> mul_trunc(const vector<int>& a, const vector<int>& b, int need) {
    auto c = convolution_mod1e9p7(a,b);
    if ((int)c.size() > need) c.resize(need);
    return c;
}

// FPS inverse: given f[0]!=0, return g s.t. f*g=1 mod x^n
static vector<int> fps_inv(const vector<int>& f, int n) {
    vector<int> g(1);
    g[0] = (int)mod_pow(f[0], MOD-2, MOD);
    int cur = 1;
    while (cur < n) {
        int nxt = min(n, cur<<1);
        vector<int> fcut(nxt);
        for (int i=0;i<nxt && i<(int)f.size();i++) fcut[i]=f[i];

        // t = fcut * g
        auto t = mul_trunc(fcut, g, nxt);
        // t = 2 - t
        for (int i=0;i<nxt;i++) {
            t[i] = (i==0 ? (2 - t[i]) : (MOD - t[i]));
            if (t[i] < 0) t[i] += MOD;
        }
        // g = g * t
        g = mul_trunc(g, t, nxt);
        cur = nxt;
    }
    g.resize(n);
    return g;
}

// build p_k(x) = sum_{j=0..floor(k/2)} (-1)^j C(k-j, j) x^j  (truncate to m)
static vector<int> build_p(int k, int m, const vector<int>& fact, const vector<int>& invfact) {
    vector<int> p(m+1, 0);
    int up = min(m, k/2);
    for (int j=0;j<=up;j++){
        // C(k-j, j) = (k-j)! / (j! (k-2j)!)
        int a = k - j;
        int b = k - 2*j;
        long long comb = 1LL * fact[a] * invfact[j] % MOD * invfact[b] % MOD;
        if (j & 1) comb = (MOD - comb) % MOD;
        p[j] = (int)comb;
    }
    return p;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    freopen("recall.in", "r", stdin);
    freopen("recall.out", "w", stdout);
    int n, m;
    cin >> n >> m;

    if (n > m) { // sum of positives >= n
        cout << 0 << "\n";
        return 0;
    }
    if (n == 1) {
        // sequences: b1>=1, b1<=m => answer = m
        cout << (m % MOD) << "\n";
        return 0;
    }

    // factorial up to n (since need (k-j)! with k up to n)
    int N = n + 5;
    vector<int> fact(N), invfact(N);
    fact[0]=1;
    for(int i=1;i<N;i++) fact[i]=(long long)fact[i-1]*i%MOD;
    invfact[N-1]=(int)mod_pow(fact[N-1], MOD-2, MOD);
    for(int i=N-2;i>=0;i--) invfact[i]=(long long)invfact[i+1]*(i+1)%MOD;

    // build p_{n}, p_{n-1}, p_{n-2}
    auto p_n   = build_p(n,   m, fact, invfact);
    auto p_nm1 = build_p(n-1, m, fact, invfact);
    auto p_nm2 = build_p(n-2, m, fact, invfact);

    // u_{n-1} = p_{n-1} / p_n
    auto inv_pn = fps_inv(p_n, m+1);
    auto u1 = mul_trunc(p_nm1, inv_pn, m+1);

    // u_{n-2} = p_{n-2} / p_{n-1}
    auto inv_pnm1 = fps_inv(p_nm1, m+1);
    auto u0 = mul_trunc(p_nm2, inv_pnm1, m+1);

    auto calc_A = [&](const vector<int>& u)->vector<int>{
        // A = (x*u) / (1 - x*u) = v * inv(1-v), v=x*u
        vector<int> v(m+1, 0);
        for (int i=0;i<m;i++) v[i+1] = u[i]; // multiply by x => shift
        vector<int> one_minus_v(m+1, 0);
        one_minus_v[0] = 1;
        for (int i=1;i<=m;i++) one_minus_v[i] = (MOD - v[i]) % MOD;
        // note: one_minus_v[0]=1 - v[0]=1 (since v0=0)
        auto inv_omv = fps_inv(one_minus_v, m+1);
        auto A = mul_trunc(v, inv_omv, m+1);
        return A;
    };

    auto A1 = calc_A(u1);
    auto A0 = calc_A(u0);

    // G_n(x) = A1 - A0
    vector<int> G(m+1,0);
    for(int i=0;i<=m;i++){
        int val = A1[i] - A0[i];
        if (val < 0) val += MOD;
        G[i] = val;
    }

    // answer = sum_{k=0..m} [x^k] G(x)
    long long ans = 0;
    for(int i=0;i<=m;i++){
        ans += G[i];
        if (ans >= MOD) ans -= MOD;
    }
    cout << ans % MOD << "\n";
    return 0;
}
