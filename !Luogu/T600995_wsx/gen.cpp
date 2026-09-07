#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

typedef long long ll;

int main(int argc, char* argv[]) {
    if (argc < 5) return 1;
    
    ll seed = atoll(argv[1]);
    ll max_n = atoll(argv[2]);
    ll max_m = atoll(argv[3]);
    ll max_val = atoll(argv[4]);

    mt19937_64 rng(seed);

    // 保证 n 为奇数
    ll n = (rng() % (max_n / 2 + 1)) * 2 + 1;
    if (n > max_n) n = max_n - (max_n % 2 == 0 ? 1 : 0);
    
    ll m = rng() % (max_m + 1);
    ll x = rng() % (max_val / 100 + 1); // 防止 x 过大导致逻辑测试失效

    cout << n << " " << m << " " << x << endl;
    for (int i = 0; i < n; ++i) {
        ll val = (rng() % (2 * max_val + 1)) - max_val;
        cout << val << (i == n - 1 ? "" : " ");
    }
    cout << endl;

    return 0;
}