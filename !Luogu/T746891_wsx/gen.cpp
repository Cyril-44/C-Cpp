#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>

using namespace std;

int main(int argc, char* argv[]) {
    ios::sync_with_stdio(false);
    cout.tie(nullptr);
    if (argc < 5) return 1;
    int n = atoi(argv[1]);
    int q = atoi(argv[2]);
    int seed = atoi(argv[3]);
    int type = atoi(argv[4]); // 0: random, 1: k=1, 2: max sequence focus

    mt19937 rng(seed);
    cout << n << " " << q << '\n';

    vector<int> a(n);
    iota(a.begin(), a.end(), 1);
    shuffle(a.begin(), a.end(), rng);

    for (int i = 0; i < n; ++i) cout << a[i] << (i == n - 1 ? "" : " ");
    cout << '\n';

    for (int i = 0; i < q; ++i) {
        int l = uniform_int_distribution<int>(1, n)(rng);
        int r = uniform_int_distribution<int>(1, n)(rng);
        if (l > r) swap(l, r);
        
        int k;
        if (type == 1) k = 1;
        else if (type == 2 && (rng() % 10 < 7)) {
            // 构造一些比较大的答案
            k = uniform_int_distribution<int>(1, n / 2)(rng);
        } else {
            k = uniform_int_distribution<int>(1, n)(rng);
        }
        cout << l << " " << r << " " << k << '\n';
    }
    return 0;
}