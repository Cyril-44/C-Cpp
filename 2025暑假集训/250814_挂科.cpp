#include <cstdio>
#include <cstring>
#include <algorithm>
constexpr int N = 1005, MOD = 1e9+7;
int a[N];
inline int qpow(int b, int n) {
    int res = 1;
    while (n) {
        if (n & 1) res = 1ll * res * b % MOD;
        b = 1ll * b * b % MOD;
        n >>= 1;
    }
    return res;
}
int f[N], C[N][N];
void* _init_C = []() -> void* {
    for (int i = 0; i < N; i++) {
        C[i][0] = C[i][i] = 1;
        for (int j = 1; j < i; j++)
            C[i][j] = (C[i-1][j-1] + C[i-1][j]) % MOD;
    }
    return nullptr;
}();
int main () {
    int t, n;
    for (scanf("%d", &t); t--; ) {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    std::sort(a+1, a+1 + n);
    f[0] = 1;
    for (int i = 1; i <= n; i++) {
        f[i] = qpow(a[i], i);
        for (int j = 1; j < i; j++) {
            (f[i] -= 1ll * f[j-1] * qpow(a[i] - a[j], i - j + 1) % MOD * C[i][j - 1] % MOD) %= MOD;
            // 在第j科就挂了 k[j~i] ∈ [a_i, a_j-1]
            if (f[i] < 0) f[i] += MOD;
        }
    }
    printf("%d\n", f[n]);
    }
    return 0;
}