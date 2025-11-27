#include <stdio.h>
const long long MOD = 20051131;
int main() {
    int n;
    scanf("%d", &n);
    long long ans = 1;
    for (int i = 1; i < n; i++)
        ans = ans * i % MOD;
    printf("%lld\n", ans);
    return 0;
}