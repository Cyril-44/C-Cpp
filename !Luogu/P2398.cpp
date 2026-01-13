#include <stdio.h>
constexpr int N = 100005;
long long g[N];
int p[N >> 2], phi[N];
bool np[N];
inline void getphi(const int n = 100000) {
    for (int i = 2; i <= n; i++) {
        if (!np[i]) p[++p[0]] = i, phi[i] = i-1;
        for (int j = 1; j <= p[0] && i * p[j] <= n; ++j) {
            np[i * p[j]] = true;
            if (i % p[j] == 0) {
                phi[i * p[j]] = phi[i] * p[j];
                break;
            }
            phi[i * p[j]] = phi[i] * phi[p[j]];
        }
    }
    g[1] = 1;
    for (int i = 2; i <= n; i++)
        g[i] = (phi[i] << 1) + g[i - 1];
}
int main() {
    getphi();
    int n;
    scanf("%d", &n);
    long long ans = 0;
    for (int i = 1; i <= n; i++)
        ans += i * g[n / i];
    printf("%lld\n", ans);
    return 0;
}