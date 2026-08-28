#include <bits/stdc++.h>
int main() {
    std::mt19937 rng(std::random_device{}());
    auto rand = [&](int l, int r) { return std::uniform_int_distribution<int>(l,r)(rng); };
    int n = 10, m = 10;
    printf("0\n%d %d\n", n, m);
    for (int i = 1; i <= n; i++) {
        int k = rand(0, m);
        printf("%d", k);
        std::vector<int> a(m);
        std::iota(a.begin(), a.end(), 1);
        std::shuffle(a.begin(), a.end(), rng);
        while (a.size() > k) a.pop_back();
        std::sort(a.begin(), a.end());
        for (int v : a) printf(" %d", v);
        putchar('\n');
    }
    int q = rand(1, 1000);
    printf("%d\n", q);
    while (q--) {
        int l = rand(1, n), r = rand(1, n);
        if (l > r) std::swap(l, r);
        int p = rand(l, r);
        printf("%d %d %d\n", l, r, p);
    }
    return 0;
}