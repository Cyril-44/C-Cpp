#include <bits/stdc++.h>
constexpr char mp[] = "AGCU";
int main() {
    std::mt19937 rng(std::random_device{}());
    using Rnd = std::uniform_int_distribution<int>;
    int n = 3;
    for (int i = 0; i < n; i++) putchar(mp[Rnd(0,3)(rng)]);
    putchar('\n');
    for (int k = 4; k--; ) printf("%d ", Rnd(0, 2)(rng));
    putchar('\n');
}