#include <bits/stdc++.h>
using namespace std;
constexpr int N = 105;
int a[N];
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T, n;
    cin >> T;
    while (T--) {
        cin >> n;
        for (int i = 1; i <= n; i++) cin >> a[i];
        sort(a+1, a+1 + n);
        int m = unique(a+1, a+1 + n) - a - 1;
        printf("%d\n", 2 * m - 1);
    }
    return 0;
}