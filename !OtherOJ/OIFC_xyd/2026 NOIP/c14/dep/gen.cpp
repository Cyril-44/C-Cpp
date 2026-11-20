#include <bits/stdc++.h>
int main() {
    std::mt19937 rng(std::random_device{}());
    using Rnd = std::uniform_int_distribution<int>;
    int T = 5, n = 2e6;
    printf("%d\n", T);
    while (T--) {
        printf("%d\n", n);
        for (int i = 1; i < n; i++) printf("%d ", Rnd(1, i)(rng));
        putchar('\n');
    }
}