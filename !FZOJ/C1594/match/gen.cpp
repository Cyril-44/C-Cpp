// gen.cpp
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n = 10; // 或者从命令行读，简单起见直接固定
    cout << n << '\n';

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> distN(1, n);

    for (int i = 1; i <= n; ++i) {
        int l = distN(rng);
        int r = distN(rng);
        if (l > r) swap(l, r);
        cout << l << ' ' << r << '\n';
    }
    return 0;
}
