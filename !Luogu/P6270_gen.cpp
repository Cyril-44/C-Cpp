#include <bits/stdc++.h>
using namespace std;
mt19937 rng(11451419);
int main() {
    ios::sync_with_stdio(false);
    uniform_int_distribution<int> Tgen(500, 1000), Ngen(6, 15), Vgen(1, 20);
    int T = Tgen(rng);
    cout << T << endl;
    while (T--) {
        int n = Ngen(rng);
        cout << n << " 114514\n";
        while (n--) {
            int l = Vgen(rng), r = Vgen(rng);
            while (r == l) r = Vgen(rng);
            if (l > r) swap(l, r);
            cout << l << ' ' << r << endl;
        }
    }
    return 0;
}