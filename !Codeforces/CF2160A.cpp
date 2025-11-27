#include <bits/stdc++.h>
using namespace std;
constexpr int N = 105;
int a[N];
int main() {
    cin.tie(nullptr) -> sync_with_stdio(false);
    int T, n;
    cin >> T;
    while (T--) {
        cin >> n;
        for (int i = 1; i <= n; i++) cin >> a[i];
        bool zero = false;
        for (int i = 1; i <= n; i++)
            if (a[i] == 0) zero = true;
        if (!zero) { puts("0"); continue; }
        std::sort(a+1, a+1 + n);
        int m = unique(a+1, a+1+n) - a-1;
        for (int i = 1; i <= m; i++) {
            if (i == m || a[i] != a[i+1] - 1) {
                printf("%d\n", a[i] + 1);
                break;
            }
        }
    }
    return 0;
}