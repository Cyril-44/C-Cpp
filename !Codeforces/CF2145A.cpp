#include <bits/stdc++.h>
using namespace std;

int main() {
    cin.tie(nullptr) -> sync_with_stdio(false);
    int t, n;
    cin >> t;
    while (t--) {
        cin >> n;
        cout << (3 - n % 3) % 3 << '\n';
    }
    return 0;
}