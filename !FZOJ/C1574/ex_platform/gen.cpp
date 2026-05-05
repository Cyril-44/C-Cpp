#include <bits/stdc++.h>
int main() {
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> nGen(5,7);
    std::uniform_int_distribution<int> mGen(1,6);
    int n = nGen(rng);
    printf("%d\n", n);
    for (int i = 1; i < n; i++) {
        int x = mGen(rng), y = mGen(rng);
        while (y == x) y = mGen(rng);
        if (y < x) std::swap(x, y);
        printf("%d %d\n", x, y);
    }
    printf("1 6\n");
    return 0;
}