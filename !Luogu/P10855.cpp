#include <stdio.h>
#include <string.h>
#include <math.h>
#include <vector>
using ULL = unsigned long long;
constexpr int MOD = (int)1e9 + 7;
inline ULL qpow(ULL b, int n) {
    ULL res = 1;
    while (n) {
        if (n & 1) res = res * b % MOD;
        b = b * b % MOD, n >>= 1;
    }
    return res;
}
constexpr int N = 200005;
int mu[N], _init_ = [](const int n = 2e5){
    mu[1] = 1;
    for (int i = 2; i <= (n >> 1); i++)
        for (int j = i << 1; j <= n; j += i)
            mu[j] -= mu[i];
return 0;}();
int f[1 << 18];
inline void add(int &x, int y) { if ((x += y) >= MOD) x -= MOD; }
int K;
inline int calc(int x, int m) { // \sum_{i=0}^m gcd(i, x)^K
    if (m < 0) return 0;
    std::vector<int> facs;
    for (int i = 1, j = sqrt(x); i <= j; i++)
        if (x % i == 0) facs.emplace_back(i);
    int pos = facs.size() - 1;
    if (facs.back() * facs.back() == x) --pos;
    for (; pos >= 0; pos--) facs.emplace_back(x / facs[pos]);
    int ans = qpow(x, K); // 计算 gcd(x, 0)
    for (int t : facs) add(ans, (ULL)(m / t) * f[t] % MOD);
    return ans;
}
int main() {
    int T, n, M;
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d", &n, &K);
        M = 1 << (32 - __builtin_clz(n+1));
        memset(f, 0, sizeof(int) * M);
        for (int i = 1; i < M; i++) {
            ULL tp = qpow(i, K);
            for (int j = i, k = 1; j < M; j += i, k++)
                add(f[j], mu[k] * tp % MOD);
        }
        int ans = 0;
        for (int x = 1; x <= n; x++) {
            // 计算 (x xor y) <= z 的 y
            auto cal = [&] (int z) {
                if (!z) return 1;
                int tx = x ^ z;
                int res = 0;
                for (int k = 32 - __builtin_clz(z); k >= 0; k--) {
                    if (z >> k & 1) {
                        printf("x = %d, Found avaliable range: [%d, %d] ^ %d <= %d\n", x, (tx>>k<<k), tx | (1<<k)-1, x, z);
                        add(res, (calc(x, tx | (1<<k)-1) - calc(x, (tx>>k<<k) - 1) + MOD) % MOD);
                        printf("Contributed %d\n", calc(x, tx | (1<<k)-1) - calc(x, (tx>>k<<k) - 1));
                        break;
                    }
                }
                printf("%d\n", res);
                return res;
            };
            add(ans, (cal(n + 1) - cal(x) + MOD) % MOD);
        }
        printf("%d\n", ans);
    }
    return 0;
}
