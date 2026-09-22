#include <bits/stdc++.h>
int main() {
    std::mt19937 rng(std::random_device{}());
    int n = 2000, q = 1e6;
    std::uniform_int_distribution<int> dist(1, n);
    printf("%d %d\n", n, q);
    while (q--) {
        std::vector<int> a(3);
        for (int &x : a) x = dist(rng);
        std::sort(a.begin(), a.end());
        for (int x : a) printf("%d ", x);
        putchar('\n');
    }
    return 0;
}