#include <bits/stdc++.h>
int main(int argc, char **argv) {
    int T = atoi(argv[1]), n = atoi(argv[2]);
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<uint64_t> aigen(1, std::atoll(argv[3]));
    printf("%d\n", T);
    while (T--) {
        printf("%d\n", n);
        for (int i = 1; i <= n; i++)
            printf("%lu ", aigen(rng));
        putchar('\n');
    }
}