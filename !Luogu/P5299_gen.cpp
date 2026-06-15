#include <bits/stdc++.h>
using namespace std;
mt19937 rng(random_device{}());
uniform_int_distribution<int> gen(1,130);
int main() {
    int n = gen(rng), m = gen(rng), k = gen(rng);
    printf("1\n%d %d %d\n", n, m, k);
    for (int i = 1; i <= n; i++) printf("%d%c", gen(rng)+1, " \n"[i==n]);
    for (int i = 1; i <= n; i++) printf("%d%c", gen(rng), " \n"[i==n]);
    return 0;
}