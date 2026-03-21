#include <bits/stdc++.h>
using namespace std;

int main() {
    freopen("maze.in", "r", stdin);
    freopen("maze.out", "w", stdout);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    cin >> T;
    while (T--) {
        int n, k;
        cin >> n >> k;
        vector<int> a(n);
        for (int &x : a) cin >> x;
        long long ans = 0;
        for (int i = 0; i <= n - k; ) {
            bool sorted = true;
            for (int j = i; j < i + k - 1; j++) {
                if (a[j] > a[j + 1]) {
                    sorted = false;
                    break;
                }
            }
            if (sorted) {
                i++;
            } else {
                ans++;
                sort(a.begin() + i, a.begin() + i + k);
                if (i > 0) i -= k - 1;
                else i = 0;
                if (i < 0) i = 0;
            }
        }
        cout << ans << '\n';
    }
    return 0;
}