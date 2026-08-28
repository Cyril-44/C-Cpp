#include <bits/stdc++.h>
int main() {
    std::mt19937 rng(std::random_device{}());
    auto rand = [&](int l, int r) { return std::uniform_int_distribution<int>(l, r)(rng); };
    int T = 1000, n = 5;
    printf("0 %d\n", T);
    while (T--) {
        printf("%d\n", n);
        for (int i = 1; i <= n; i++)
            printf("%d%c", rand(1, i), " \n"[i==n]);
    }
    return 0;
}