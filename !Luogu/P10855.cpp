#include <stdio.h>
#include <string.h>
#include <math.h>
#include <vector>
using LL = long long;
using ULL = unsigned long long;
constexpr int MOD = (int)1e9 + 7;
inline int qpow(ULL b, int n) {
    ULL res = 1;
    while (n) {
        if (n & 1) res = res * b % MOD;
        b = b * b % MOD, n >>= 1;
    }
    return res;
}
constexpr int N = 200005;
std::vector<int> facs[N];
int mu[N], _init_ = [](const int n = 2e5){
    mu[1] = 1;
    for (int i = 1; i <= (n >> 1); i++)
        for (int j = i << 1; j <= n; j += i)
            mu[j] -= mu[i];
    for (int i = 1; i <= n; i++)
        for (int j = i; j <= n; j += i)
            facs[j].push_back(i);
return 0;}();
int f[N];
int K;
[[gnu::always_inline]] inline void add(int &x, int y) { if ((x += y) >= MOD) x -= MOD; }
[[gnu::always_inline]] inline void sub(int &x, int y) { if ((x -= y) < 0) x += MOD; }
int main() {
    int T, n;
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d", &n, &K);
        memset(f, 0, sizeof(int) * (n+1));
        for (int i = 1; i <= n; i++) {
            int tp = qpow(i, K);
            for (int j = i, k = 1; j <= n; j += i, k++)
                if (mu[k] > 0) add(f[j], tp);
                else if (mu[k] < 0) sub(f[j], tp);
        }
        int ans = 0;
        for (int x = 1; x <= n; x++) { // 计算 (x xor y) <= z 的 y
            auto calc = [x] (int m) { // \sum_{i=0}^m gcd(i, x)^K
                if (m < 0) return 0;
                int res = qpow(x, K); // 计算 gcd(x, 0)^K
                for (int t : facs[x]) 
                    add(res, (ULL)(m / t) * (ULL)f[t] % (ULL)MOD);
                return res;
            };
            auto cal = [&] (int z) { // 计算 \sum_j [x <= x xor j <= n] * gcd(x, j)^K
                if (!z) return 1;
                int res = 0;
                int tx = x;
                for (int k = 31 - __builtin_clz(z); k >= 0; k--) {
                    if (z >> k & 1) {
                        // printf("tx = %d, Found avaliable range: [%d, %d] ^ %d <= %d\n", tx, (tx>>k<<k), tx | (1<<k)-1, tx, z);
                        add(res, calc(tx | (1<<k)-1));
                        sub(res, calc((tx>>k<<k) - 1));
                        // printf("Contributed %d\n", calc(tx | (1<<k)-1) - calc((tx>>k<<k) - 1));
                        tx ^= 1 << k;
                    }
                }
                return res;
            };
            add(ans, cal(n + 1)); sub(ans, cal(x));
        }
        printf("%d\n", ans);
    }
    return 0;
}
