#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;

// 快速幂
long long qpow(long long a, long long b) {
    long long r = 1;
    while (b) {
        if (b & 1) r = r * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return r;
}

// 预处理组合数 C(n, k) 到某个上界
struct Comb {
    int N;
    vector<long long> fact, invfact;
    Comb(int n = 0) { init(n); }
    void init(int n) {
        N = n;
        fact.assign(N + 1, 1);
        invfact.assign(N + 1, 1);
        for (int i = 1; i <= N; ++i) fact[i] = fact[i - 1] * i % MOD;
        invfact[N] = qpow(fact[N], MOD - 2);
        for (int i = N; i > 0; --i) invfact[i - 1] = invfact[i] * i % MOD;
    }
    long long C(int n, int k) {
        if (n < 0 || k < 0 || k > n) return 0;
        return fact[n] * invfact[k] % MOD * invfact[n - k] % MOD;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n, m;
    int k;
    if (!(cin >> n >> m >> k)) return 0;
    vector<long long> c(k);
    for (int i = 0; i < k; ++i) cin >> c[i];

    sort(c.begin(), c.end());

    // 计算每个 i 的下界 L_i = ceil((n - k + i) / 2)
    vector<long long> L(k + 1);
    for (int i = 1; i <= k; ++i) {
        long long num = n - k + i;
        L[i] = (num + 1) / 2; // ceil
    }

    // 这里为了演示，把 T 的范围粗暴限制在 [0, n]，
    // 实际要做优化的话，需要更精细的剪枝。
    int maxT = (int)n;

    // 预处理组合数到 n + m（星与棒里会用到）
    Comb comb(n + m + 5);

    // dp[i][t]：处理到第 i 个 c_i，前缀 <= c_i 的元素数为 t 的方案数
    vector<long long> dp_prev(maxT + 1, 0), dp_cur(maxT + 1, 0);
    dp_prev[0] = 1; // T_0 = 0

    long long c0 = 0;
    for (int i = 1; i <= k; ++i) {
        fill(dp_cur.begin(), dp_cur.end(), 0);
        long long ci = c[i - 1];
        long long len = ci - c0; // 段 (c_{i-1}, c_i] 的长度
        for (int t = (int)L[i]; t <= maxT; ++t) { // T_i >= L_i
            long long ways = 0;
            // 枚举 T_{i-1} = t'
            for (int tp = 0; tp <= t; ++tp) {
                if (!dp_prev[tp]) continue;
                long long add = t - tp; // 这一段要放的元素数
                // 星与棒：len 个值，放 add 个元素
                long long w = comb.C(add + len - 1, add);
                ways = (ways + dp_prev[tp] * w) % MOD;
            }
            dp_cur[t] = ways;
        }
        dp_prev.swap(dp_cur);
        c0 = ci;
    }

    // 最后一段 (c_k, m]，长度 len_last = m - c_k
    long long ck = c[k - 1];
    long long len_last = m - ck;

    long long ans = 0;
    for (int t = 0; t <= maxT; ++t) {
        if (!dp_prev[t]) continue;
        long long rem = n - t; // 剩下的元素数
        if (rem < 0) continue;
        long long w = comb.C(rem + len_last - 1, rem);
        ans = (ans + dp_prev[t] * w) % MOD;
    }

    cout << ans % MOD << "\n";
    return 0;
}
