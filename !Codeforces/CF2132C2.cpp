#include <bits/stdc++.h>
using namespace std;
constexpr int K = 32;
long long val[K];
int cnt[K];
int main() {
    int t, n, k;
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d", &n, &k);
        long long base = 3;
        int exp = 0, cntSum = 0;
        for (int tp = n, i = 0; tp; ++i) {
            val[i] = (base + exp * (base / 9));
            cntSum += cnt[i] = tp % 3;
            base *= 3, ++exp, tp /= 3;
        }
        if (cntSum > k) { puts("-1"); continue; }
        k -= cntSum;
        for (int i = exp-1; i > 0; i--) {
            int sub = min(k / 2, cnt[i]);
            k -= 2*sub, cnt[i] -= sub, cnt[i-1] += 3 * sub;
        }
        long long ans = 0;
        for (int i = 0; i < exp; i++)
            ans += cnt[i] * val[i];
        printf("%lld\n", ans);
    }
    return 0;
}