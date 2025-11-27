#include <bits/stdc++.h>
using namespace std;
constexpr int N = 100005;
int a[N];
typedef long long LL;
int main() {
    int T, n, k;
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d", &n, &k);
        for (int i = 1; i <= n; i++)
            scanf("%d", a + i);
        if (k & 1) {
            for (int i = 1; i <= n; i++)
                if (a[i] & 1) printf("%d ", a[i] + k);
                else printf("%d ", a[i]);
            putchar('\n');
        } else {
            for (int i = 1; i <= n; i++) {
                printf("%lld ", a[i] + 1ll * k * (a[i] % (k+1)));
            }
            putchar('\n');
        }
    }
    return 0;
}