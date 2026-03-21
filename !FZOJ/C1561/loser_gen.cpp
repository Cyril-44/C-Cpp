#include <bits/stdc++.h>
int main(int argc, char** argv) {
    std::mt19937 rng(std::random_device{}());
    int n = 8000;
    if (argc == 2) n = atoi(argv[1]);
    printf("%d\n", n);
    for (int i = 1; i <= n; i++)
        putchar(rng() & 1 | '0'), putchar(' ');
    putchar('\n');
    for (int i = 1; i < n; i++)
        printf("%d %d\n", i, i+1);
    return 0;
}