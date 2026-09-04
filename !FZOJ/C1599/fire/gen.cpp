#include <bits/stdc++.h>
int main() {
    std::mt19937 rng(std::random_device{}());
    auto rnd = [&](int l, int r) { return std::uniform_int_distribution<int>(l, r)(rng); };
    int n = 4000, q = 50000;
    printf("%d %d\n", n, q);
    std::vector<int> typ{1,1,1,1,2};
    while (q--) {
        std::shuffle(typ.begin(), typ.end(), rng);
        int type = typ[0];
        printf("%d", type);
        if (type == 1) printf(" %d %d %d %d\n", rnd(0, 1), rnd(1, n), rnd(1, 4), rnd(1, (int)1e9));
        else printf(" %d %d\n", rnd(1, n), rnd(1, n));
    }
}