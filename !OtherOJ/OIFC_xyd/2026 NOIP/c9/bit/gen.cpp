#include <bits/stdc++.h>
int main() {
    std::mt19937 rng(std::random_device{}());
    using Rnd = std::uniform_int_distribution<int>;
    int n = 8, q = 20;
    printf("%d %d\n", n, q);
    for (int i = 2; i <= n; i++)
        printf("%d %d\n", i, Rnd(1, i-1)(rng));
    while (q--) {
        int op = Rnd(0,2)(rng)%2?1:2;
        printf("%d ", op);
        if (op == 1) {
            int l = Rnd(1, n)(rng), r = Rnd(1, n)(rng);
            if (l > r) std::swap(l, r);
            printf("%d %d\n", l, r);
        }
        else printf("%d\n", Rnd(1, n-1)(rng));
    }
}