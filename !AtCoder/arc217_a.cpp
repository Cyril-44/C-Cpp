#include <cstdio>
#include <cstring>
#include <numeric>
#include <algorithm>
constexpr int N = 100005;
int p[N], a[N];
int main() {
    int n;
    scanf("%d", &n);
    int minsum = 1e9;
    std::iota(p, p+n, 1);
    do {
        int xorsum = 0, sum = 0;
        for (int i = 0; i < n; i++) {
            xorsum ^= p[i];
            sum += xorsum;
        }
        if (sum < minsum) {
            minsum = sum;
            memcpy(a, p, sizeof(int) * n);
        }
    } while (std::next_permutation(p, p+n));
    printf("XorSum = %d\n", minsum);
    for (int i = 0; i < n; i++)
        printf("%d ", a[i]);
    putchar('\n');
    return 0;
}