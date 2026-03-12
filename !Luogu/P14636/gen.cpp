#include <bits/stdc++.h>
int main() {
    std::mt19937_64 rng(std::random_device{}());
    std::uniform_int_distribution<int> rand(0, 8), rand1(1, 5);
    int c = 1, T = 5, n = 6;
    printf("%d %d\n", c, T);
    while (T--) {
        printf("%d %d\n", n, rand(rng));
        for (int i = 1; i <= n; i++)
            printf("%d ", rand1(rng));
        putchar('\n');
    }
    return 0;
}