#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <vector>
constexpr int N = 500005;
constexpr int MOD = 15112009;
using ULL = unsigned long long;
unsigned qpow(unsigned b, unsigned n) {
    unsigned res = 1;
    while (n) {
        if (n & 1) res = (ULL)res * b % MOD;
        b = (ULL)b * b % MOD;
        n >>= 1;
    }
    return res;
}
unsigned f[N][4];
int a[N];
int main() {
    freopen("decryption.in", "r", stdin);
    freopen("decryption.out", "w", stdout);
    int n;
    scanf("%d", &n);
    std::vector<int> zeropos;
    zeropos.reserve(n);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &a[i]);
        if (!a[i]) zeropos.push_back(i);
    }
    if (zeropos.empty()) {
        printf("%u\n", qpow(2, n-1));
        return 0;
    }
    int ans0 = 1;
    for (size_t i = 1; i < zeropos.size(); i++)
        ans0 = (ULL)ans0 * (zeropos[i] - zeropos[i-1] + 1) % MOD;
    f[0][0] = 1;
    for (int i = 1; i <= n; i++) {
        switch (a[i]) {
        case 0:
            f[i][1] = (f[i-1][0] + f[i-1][1] + f[i-1][3]) % MOD;
            f[i][3] = (f[i-1][2] + f[i-1][3]) % MOD;
            break;
        case 1:
            f[i][2] = (f[i-1][0] + f[i-1][2] + f[i-1][3]) % MOD;
            f[i][3] = (f[i-1][1] + f[i-1][3]) % MOD;
            break;
        default:
            f[i][0] = (f[i-1][0] + f[i-1][3]) % MOD;
            f[i][1] = f[i-1][1];
            f[i][2] = f[i-1][2];
            f[i][3] = f[i-1][3];
        }
    }
    printf("%d\n", (ans0 - f[n][3] + MOD) % MOD);
    return 0;
}