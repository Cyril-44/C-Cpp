#include <bits/stdc++.h>
constexpr int N = 1000005;
int p[N], q[N];
int main() {
    std::mt19937 rng(std::random_device{}());
    int n = 1e6;
    std::uniform_int_distribution<int> ngen(1, n), sgen(0, n);
    std::iota(p+1, p+1+n, 1);
    std::shuffle(p+1, p+1+n, rng);
    std::iota(q+1, q+1+n, 1);
    std::shuffle(q+1, q+1+n, rng);
    int rnd = sgen(rng);
    while (rnd--) q[ngen(rng)] = -1;
    printf("%d\n", n);
    for (int i = 1; i <= n; i++) printf("%d%c", p[i], " \n"[i==n]);
    for (int i = 1; i <= n; i++) printf("%d%c", q[i], " \n"[i==n]);
}