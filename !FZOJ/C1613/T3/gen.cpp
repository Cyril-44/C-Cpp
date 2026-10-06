#include <bits/stdc++.h>
int main() {
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> u(0, 15);
    int n = 10;
    printf("%d\n", n);
    for (int i = 1; i <= n; i++)
        printf("%d%c", u(rng), " \n"[i==n]);
}