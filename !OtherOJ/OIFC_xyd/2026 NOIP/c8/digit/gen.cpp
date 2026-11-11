#include <bits/stdc++.h>
int main() {
    std::mt19937 rng(std::random_device{}());
    using Rnd = std::uniform_int_distribution<int>;
    int n = 5, q = 8;
    printf("%d %d\n", n, q);
    for (int i = 1; i <= n; i++)
        printf("%d%c", Rnd(1, 1000)(rng), " \n"[i==n]);
    while (q--) {
        int l = Rnd(1, n)(rng), r = Rnd(1, n)(rng);
        if (l > r) std::swap(l, r);
        printf("%d %d %d\n", Rnd(1,2)(rng), l, r);
    }
}