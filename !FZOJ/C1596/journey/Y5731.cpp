#include <cstdio>
constexpr int N = 500004;
int tid, n, a[N];
int main() {
    scanf("%d%d", &tid, &n);
    for (int i = 1; i <= n; i++)
        for (int j = i; j <= n; j += i)
            a[j] ^= (j / i - 1) * i ^ j;
    for (int i = 1; i <= n; i++) {
        a[i] ^= a[i - 1];
        printf("%d\n", a[i]);
    }
    return 0;
}