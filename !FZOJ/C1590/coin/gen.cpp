#include <cstdio>
#include <random>
int main() {
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> uid(1, 10);
    int n = 6, m = 8;
    printf("%d %d\n", n, m);
    for (int i = 1; i <= n; i++) printf("%d%c", uid(rng), " \n"[i==n]);    
    for (int i = 1; i <= n; i++) printf("%d%c", uid(rng), " \n"[i==n]);    
    return 0;
}