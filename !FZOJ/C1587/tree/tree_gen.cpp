#include <bits/stdc++.h>
int main() {
    std::mt19937 rng(std::random_device{}());
    auto rand = [&](int l, int r) { return std::uniform_int_distribution<int>(l, r)(rng); };
    int T = 1000;
    int n = 20, t = 8;
    printf("%d\n", T);
    while (T--) {
        printf("%d %d\n", n, t);
        for (int i = t; i < n; i++)
            printf("%d%c", rand(1, i), " \n"[i==n-1]);
        for (int i = 1; i <= n; i++)
            printf("%d%c", rand(0, 1), " \n"[i==n]);
    }
    return 0;
}