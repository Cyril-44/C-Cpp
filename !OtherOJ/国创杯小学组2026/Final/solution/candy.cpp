#include <bits/stdc++.h>
using namespace std;
int main() {
    freopen("candy.in", "r", stdin);
    freopen("candy.out", "w", stdout);
    long long n, x, m, k;
    cin >> n >> x >> m >> k;
    if (n < m * k)
        cout << -1;
    else
        cout << (n - m * k) * x;
    return 0;
}