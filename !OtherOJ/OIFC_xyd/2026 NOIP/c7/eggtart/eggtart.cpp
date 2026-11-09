#include <bits/stdc++.h>
constexpr int N = 1e8;
std::bitset<N*2> vis;
int a[10005];
int main() {
    freopen("eggtart.in", "r", stdin);
    freopen("eggtart.out", "w", stdout);
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> rnd(0, N-1);
    int n; scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        int val; bool flg = false;
        do {val = rnd(rng);
            flg = false;
            for (int j = 0; !flg && j < i; j++)
                if (vis._Unchecked_test(a[j] ^ a[i]))
                    flg = true;
        } while (flg);
        for (int j = 0; j < i; j++)
            vis._Unchecked_set(a[j] ^ a[i]);
        printf("%d ", val);
    }
    putchar('\n');
}