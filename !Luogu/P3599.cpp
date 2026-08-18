#include <cstdio>
#include <cassert>
constexpr int N = 100005;
int a[N], inv[N];
bool np[N];
int main() {
    for (int i = 2; i*i <= 100000; i++) if (!np[i])
        for (int j = i*i; j <= 100000; j += i)
            np[j] = true;
    int X, T, n;
    scanf("%d%d", &X, &T);
    while (T--) {
        scanf("%d", &n);
        if (X == 1) {
            if (n == 1) puts("2 1");
            else if (n % 2 == 1) puts("0");
            else {
                putchar('2');
                for (int i = 0; i < n; i++) {
                    if (i & 1) printf(" %d", i);
                    else printf(" %d", n - i);
                }
                putchar('\n');
            }
        } else {
            switch (n) {
            case 1: puts("2 1");       break;
            case 4: puts("2 1 3 2 4"); break;
            default:
                if (np[n]) puts("0");
                else {
                    inv[1] = 1;
                    for (int i = 2; i < n; i++) inv[i] = 1ull * (n - n / i) * inv[n % i] % n;
                    printf("2 1");
                    for (int i = 2; i <= n; i++) printf(" %d", 1 + inv[i-1]);
                    putchar('\n');
                }
            }
        }
    }
    return 0;
}