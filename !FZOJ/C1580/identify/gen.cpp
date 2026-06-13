#include <bits/stdc++.h>
constexpr int N = 100005;
std::random_device rd;
int main() {
    std::mt19937 rng(rd());
#ifdef SMALL
    int n = 5000;
#else
    int n = 100000;
#endif
    printf("%d\n", n);
    for (int i = 1; i <= n; i++)
        printf("%d%c", rng() & 1 ? 1 : -1, " \n"[i==n]);
    return 0;
}