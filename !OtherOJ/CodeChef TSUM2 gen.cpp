#include <bits/stdc++.h>
using namespace std;
mt19937 rng(random_device{}());
inline int rnd(int l, int r) {
    return uniform_int_distribution<int>(l,r)(rng);
}
int main() {
    int T = rnd(200, 500);
    printf("%d\n", T);
    while (T--) {
        int n = rnd(10000, 40000);
        printf("%d\n", n);
        for (int i = 1; i <= n; i++)
            printf("%d ", rnd(-1000, 1000));
        for (int i = 2; i <= n; i++)
            printf("%d %d\n", rnd(1, i-1), i);
    }
}