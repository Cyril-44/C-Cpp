#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T, n, rk, ck, rd, cd;
    cin >> T;
    while (T--) {
        cin >> n >> rk >> ck >> rd >> cd;
        int r = (rk == rd) ? abs(ck - cd) : rk > rd ? (n - rd) : rd;
        int c = (ck == cd) ? abs(rk - rd) : ck > cd ? (n - cd) : cd;
        cout << max(c, r) << '\n';
    }
    return 0;
}