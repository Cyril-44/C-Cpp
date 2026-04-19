#include <bits/stdc++.h>
using namespace std;
int main() {
    mt19937 rng(random_device{}());
    int n = 4, m = 5;
    printf("%d %d\n", n, m);
    uniform_int_distribution<int> MGen(1, m);
    uniform_int_distribution<int> WGen(1, 4);
    vector<int> pos(n);
    iota(pos.begin(), pos.end(), 1);
    auto get = [&]() {
        shuffle(pos.begin(), pos.end(), rng);
        int res = pos.back();
        pos.pop_back();
        return res;
    };
    for (int i = 1; i <= n; i++) {
        int type = (rng() & 1) + 1;
        int l = MGen(rng), r = MGen(rng);
        if (l > r) swap(l, r);
        int p = rng() & 1 ? get() : 0;
        if (type == 1) {
            int w = WGen(rng);
            printf("%d %d %d %d %d\n", type, l, r, w, p);
        } else {
            printf("%d %d %d %d\n", type, l, r,p);
        }
    }
    return 0;
}