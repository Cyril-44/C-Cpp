#include <bits/stdc++.h>
#include <random>
int main() {
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int64_t> ai(0, 1e18), mm(1, 1e9);
    int n = 2000, m = mm(rng);
    printf("%d %d\n", n, m);
    for (int i = 1; i <= n; i++) printf("%ld ", ai(rng));
    putchar('\n');
    for (int i = 2; i <= n; i++)
        printf("%d %d\n", i, std::uniform_int_distribution<int>(1, 1)(rng));
}