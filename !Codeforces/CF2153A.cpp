#include <bits/stdc++.h>
using namespace std;
constexpr int N = 105;
int b[N];
int main() {
    cin.tie(nullptr) -> sync_with_stdio(false);
    int T, n;
    cin >> T;
    while (T--) {
        cin >> n;
        for (int i = 1; i <= n; i++) cin >> b[i];
        sort(b+1, b+1 + n);
        int m = unique(b+1, b+1+n) - b - 1;
        cout << m << '\n';
    }
    return 0;
}