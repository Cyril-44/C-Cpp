#include <bits/stdc++.h>
int main() {
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> u(1, (int)1e9);
    int n = 5e5;
    printf("%d\n", n);
    for (int i = 1; i <= n; i++) {
        int l = u(rng), r = u(rng);
        if (l > r) std::swap(l, r);
        printf("%d %d\n", l, r);
    }
    return 0;
}