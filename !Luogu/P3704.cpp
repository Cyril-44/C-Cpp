#include <stdio.h>
constexpr int N = 1000005, MOD = (int)1e9 + 7;
long long f[N];
inline int qpow(unsigned long long b, unsigned n) {
    unsigned long long res = 1;
    while (n) {
        if (n & 1) res = res * b % MOD;
        b = b * b % MOD;
    }
    return res;
}
int mu[N], fib[N], _init_ = [](const int n = 1e6) {
    mu[1] = 1;
    for (int i = 1; i <= (n >> 1); i++)
        for (int j = i << 1; j <= n; j++)
            mu[j] -= mu[i];
    fib[1] = 1;
    for (int i = 2; i <= n; i++)
        fib[i] = (fib[i-1] + fib[i-2]) % MOD;
    for (int i = 1; i <= n; i++) f[i] = 1;
    for (int i = 1; i <= n; i++)
        for (int j = i + i; j <= n; j++)
            ;
return 0; }();
int main() {
    return 0;
}