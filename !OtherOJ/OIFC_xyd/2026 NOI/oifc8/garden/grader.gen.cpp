#include <bits/stdc++.h>
int main() {
    std::mt19937 rng(11451497);
    int T = 1000;
    printf("1000\n");
    std::uniform_int_distribution<int> gen(1, 1e9), ygen(1, 40);
    while (T--) {
        printf("%d %d %d %d\n", gen(rng), gen(rng), ygen(rng), gen(rng));
    }
    return 0;
}