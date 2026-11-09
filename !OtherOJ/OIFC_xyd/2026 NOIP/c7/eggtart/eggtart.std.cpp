#include <iostream>
using namespace std;
int mul(int a, int b) {
    int ret = 0;
    for (int i = 12; i >= 0; --i) {
        ret <<= 1;
        if (b & (1 << i)) ret ^= a;
        if (ret & 0x2000) ret ^= 0x201B;
    }
    return ret;
}
int main() {
    freopen("eggtart.in", "r", stdin);
    freopen("eggtart.out", "w", stdout);
    int n;
    cin >> n;
    for (int i = 1; i <= n; ++i) cout << ((mul(i, mul(i, i)) << 13) | i) << " ";
}