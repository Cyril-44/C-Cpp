#include <bits/stdc++.h>
constexpr int N = 200000;
int main() {
    int n = N;
    std::vector<int> a(n);
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<unsigned> dist(1u, (1u<<31) / n);
    printf("%d\n", n);
    for (int i = 1; i <= n; i++)
        printf("%u ", dist(rng));
    putchar('\n');
    return 0;
}