#include <bits/stdc++.h>
#include <random>
int main() {
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<> ui(1, 1e9);
    int T = 5;
    printf("%d\n", T);
    while (T--) {
        int n = 2e5;
        printf("%d\n", n);
        while (n--) printf("%d ", ui(rng));
        putchar('\n');
    }
}