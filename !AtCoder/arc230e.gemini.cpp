#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

static const int MOD = 998244353;

long long power(long long base, long long exp) {
    long long res = 1;
    base %= MOD;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % MOD;
        base = (base * base) % MOD;
        exp /= 2;
    }
    return res;
}

long long modInverse(long long n) {
    return power(n, MOD - 2);
}

struct Combinatorics {
    int n;
    vector<long long> fact, invFact;

    Combinatorics(int n) : n(n), fact(n + 1), invFact(n + 1) {
        fact[0] = 1;
        invFact[0] = 1;
        for (int i = 1; i <= n; i++) {
            fact[i] = (fact[i - 1] * i) % MOD;
        }
        invFact[n] = modInverse(fact[n]);
        for (int i = n - 1; i >= 1; i--) {
            invFact[i] = (invFact[i + 1] * (i + 1)) % MOD;
        }
    }

    long long C(int n, int k) const {
        if (k < 0 || k > n) return 0;
        return fact[n] * invFact[k] % MOD * invFact[n - k] % MOD;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    if (!(cin >> N >> M)) return 0;

    int K = M - 1;

    // 边界情况 1：不可能超标
    if (K >= 2 * N - 1) {
        cout << N << "\n";
        return 0;
    }

    int L = K - N;

    // 边界情况 2：没有绝对安全球
    if (L <= 0) {
        cout << 0 << "\n";
        return 0;
    }

    Combinatorics comb(N + 10);

    // 1. 计算成功进入安全区的总概率 P_safe
    long long P_safe = (long long)L * modInverse(N) % MOD; // k = 0

    long long prob_k1 = (long long)L * modInverse((long long)N * (N - 1) % MOD) % MOD;
    P_safe = (P_safe + (long long)(N - L) * prob_k1) % MOD; // k = 1

    for (int m = L + 1; m <= K / 2; m++) {
        int len_m = K - 2 * m;
        if (len_m < 0) continue;

        long long term1 = (long long)L * modInverse((long long)(N - len_m) * (N - len_m - 1) % MOD) % MOD;
        long long term2 = prob_k1;
        long long diff = (term1 - term2 + MOD) % MOD;
        P_safe = (P_safe + diff) % MOD;
    }

    // 2. 计算 P(score >= V) 的和
    long long sum_P = 0;

    for (int V = L + 1; V <= N; V++) {
        int nI = N - V + 1;
        int min_val = min(K - V + 1, V);
        int nJ = N - min_val + 1;

        // k = 0
        long long P_V = (long long)L * modInverse(N) % MOD * nI % MOD * modInverse(nJ) % MOD;

        // k = 1
        for (int m = L + 1; m <= N; m++) {
            int nI_prime = nI - (m >= V ? 1 : 0);
            int nJ_prime = nJ - (m >= min_val ? 1 : 0);
            long long term = (long long)nI_prime * modInverse(nJ_prime) % MOD * prob_k1 % MOD;
            P_V = (P_V + term) % MOD;
        }

        // k >= 2
        for (int m = L + 1; m <= K / 2; m++) {
            int len_m = K - 2 * m;
            if (len_m < 0) continue;

            int J_cap = max(0, K - m - max(m + 1, min_val) + 1);
            int I_cap = max(0, K - m - max(m + 1, V) + 1);
            int W = len_m - J_cap;

            int nI_prime = nI - (m >= V ? 1 : 0);
            int nJ_prime = nJ - (m >= min_val ? 1 : 0);

            long long inv_J = (J_cap > 0) ? modInverse(J_cap) : 0;

            for (int bJ = 0; bJ <= J_cap; bJ++) {
                long long num_sub = (J_cap > 0) ? (bJ * I_cap % MOD * inv_J % MOD) : 0;
                long long num = (nI_prime - num_sub + MOD) % MOD;
                long long den = nJ_prime - bJ;

                long long val = num * modInverse(den) % MOD;

                int N_minus_W = N - W;
                long long f_bJ = (long long)L * modInverse((long long)N_minus_W * comb.C(N_minus_W - 1, bJ + 1) % MOD) % MOD;

                long long term = comb.C(J_cap, bJ) * val % MOD * f_bJ % MOD;

                if (bJ == 0) {
                    long long subtract_k1 = (long long)nI_prime * modInverse(nJ_prime) % MOD * prob_k1 % MOD;
                    term = (term - subtract_k1 + MOD) % MOD;
                }

                P_V = (P_V + term) % MOD;
            }
        }

        sum_P = (sum_P + P_V) % MOD;
    }

    long long ans = ((long long)L * P_safe % MOD + sum_P) % MOD;
    cout << ans << "\n";

    return 0;
}