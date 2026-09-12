#include <bits/stdc++.h>
using namespace std;

constexpr int MOD = 998244353;
struct Mint {
    Mint& operator+=(Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint x, Mint y) { return x += y; }
    friend Mint operator-(Mint x, Mint y) { return x -= y; }
    friend Mint operator*(Mint x, Mint y) { return x *= y; }
    Mint(int v=0) : val(v) {}
    int val;
};
Mint qpow(Mint b, int n) {
    Mint res = 1;
    while (n) {
        if (n & 1) res *= b;
        b *= b, n >>= 1;
    }
    return res;
}

vector<Mint> fact, invfact;

void initComb(int n) {
    fact.assign(n + 1, Mint(1));
    for (int i = 1; i <= n; i++) fact[i] = fact[i - 1] * Mint(i);
    invfact.assign(n + 1, Mint(1));
    invfact[n] = qpow(fact[n], MOD-2);
    for (int i = n; i >= 1; i--) invfact[i - 1] = invfact[i] * Mint(i);
}

Mint C(long long n, long long r) {
    if (r < 0 || n < 0 || r > n) return Mint(0);
    return fact[n] * invfact[r] * invfact[n - r];
}

int main() {
    long long N, M, K;
    cin >> N >> M >> K;
    vector<long long> c(K + 2);
    for (int i = 1; i <= K; i++) cin >> c[i];
    sort(c.begin() + 1, c.begin() + K + 1);
    c[0] = 0;
    c[K + 1] = M + 1;

    long long Np = N - K;           // N'
    if (Np < 0) { cout << 0 << "\n"; return 0; }

    initComb((int)(Np + M + 5));

    // 未加限制的总数：从 M 个值里选 N' 个 multiset
    Mint ans = C(Np + M - 1, Np);

    for (long long k = 0; k <= K; k++) {
        long long H = c[k + 1] - c[k] - 1;   // 区间内可取值的个数
        if (H <= 0) continue;

        long long A = (Np + k) / 2 + 1;      // A_k = floor((N'+k)/2) + 1
        if (A > Np) continue;                // 尾和为空，Bad_k = 0

        Mint bad(0);
        long long R = M - H;                 // 区间外可取值个数（无需单独用，仅示意）
        for (long long x = 0; x < H; x++) {
            Mint left  = C(x + A - 1, x);                       // 到 (x, A-1) 的路径数
            Mint right = C((M - 1 - x) + (Np - A), M - 1 - x);  // 从 (x, A) 到终点
            bad = bad + left * right;
        }
        ans = ans - bad;
    }

    cout << ans.val << "\n";
    return 0;
}