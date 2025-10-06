#include <bits/stdc++.h>
using namespace std;
constexpr int N = 35;
int n, k;
int f[N];
bool dfs(int p) {
    if (p > n) {
        int ans = 0;
        for (int l = 1; l <= f[0]; ++l)
            for (int i = 1, j = l; j <= f[0]; ++i, ++j)
                ans += ((l&1) ? 1 : -1) * f[i] * (n - f[j]);
        return ans == k;
    }
    if (p < n) {
        f[++f[0]] = p;
        if (dfs(p + 2)) return true;
        --f[0];
    }
    return dfs(p + 1);
}
int main() {
    cin.tie(nullptr) -> sync_with_stdio(false);
    int T;
    cin >> T;
    while (T--) {
        cin >> n >> k;
        memset(f, 0, sizeof f);
        if (dfs(1)) {
            int p = 1;
            for (int i = 1; i <= f[0]; i++) {
                for (; p < f[i]; p++)
                    cout << p << ' ';
                cout << f[i]+1 << ' ' << f[i] << ' ';
                p = f[i]+2;
            }
            for (; p <= n; p++) cout << p << ' ';
            cout << '\n';
        }
        else cout << "-1\n";
    }
    return 0;
}