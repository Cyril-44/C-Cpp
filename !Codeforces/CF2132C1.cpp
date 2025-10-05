#include <bits/stdc++.h>
using namespace std;
constexpr int K = 32;
int main() {
    int t, n;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        long long ans = 0;
        long long base = 3;
        int exp = 0;
        while (n) {
            ans += (n % 3) * (base + exp * (base / 9));
            base *= 3, ++exp, n /= 3;
        }
        printf("%lld\n", ans);
    }
    return 0;
}
// 3^4 + 3*3^3 3*(3)