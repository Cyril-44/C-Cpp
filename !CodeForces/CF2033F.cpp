#include <cstdio>
#include <cstring>
#include <stdlib.h>
const int N = 600005, MOD = 1000000007;
int fib[N], id[N];
int main() {
    int t, k;
    long long n;
    scanf("%d", &t);
    while (t--) {
        scanf("%lld%d", &n, &k);
        if (k == 1) {
            printf("%lld\n", n % MOD);
            continue;
        }
        fib[1] = 1, fib[2] = 1;
        int cnt = 0, len;
        for (int i = 3; ; i++) {
            fib[i] = (fib[i-1] + fib[i-2]) % k;
            if (fib[i] == 0) id[cnt++] = i;
            if (fib[i] == 1 && fib[i-1] == 1) {
                len = i-2; break;
            }
        }
        printf("%lld\n", ((n - 1ll) / cnt % MOD * len % MOD + id[(n-1) % cnt]) % MOD);
    }
    return 0;
}