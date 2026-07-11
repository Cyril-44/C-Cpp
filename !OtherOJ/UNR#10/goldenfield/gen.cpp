#include <bits/stdc++.h>
int main() {
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> uid(1, 10);
    int n = uid(rng), q = uid(rng);
    printf("%d %d\n", n, q);
    for (int i = 1; i <= n; i++) printf("%d ", uid(rng));
    putchar('\n');
    std::uniform_int_distribution<int> uidn(1, n);
    while (q--) {
        int l = uidn(rng), r = uidn(rng);
        if (l > r) std::swap(l, r);
        printf("%d %d\n", l, r);
    }
    return 0;
}