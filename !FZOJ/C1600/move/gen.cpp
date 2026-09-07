#include <bits/stdc++.h>
int main() {
    std::mt19937 rng(std::random_device{}());
    auto rnd = [&](int l, int r) { return std::uniform_int_distribution<int>(l,r)(rng); };
    int n = 10;
    printf("%d\n", n);
    for (int i = 2; i <= n; i++)
        printf("%d %d %d\n", rnd(1, i-1), i, rnd(-5, 5));
    return 0;
}