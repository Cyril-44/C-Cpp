#include <bits/stdc++.h>
using namespace std;
constexpr int N = 1005;
int a[N];
int f[N];
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T, n;
    cin >> T;
    while (T--) {
        cin >> n;
        memset(a, 0, sizeof(int) * (n+1));
        memset(f, 0, sizeof(int) * (n+3));
        for (int i = n; i > 1; i -= 2) {
            int res1, res2;
            cout << "throw " << i-1 << endl;
            cin >> res1;
            cout << "swap " << i-1 << endl;
            cout << "throw " << i-1 << endl;
            cin >> res2;
            if (res1 == f[i+1] + 1 && res2 == f[i+1] + 1) a[i-1] = a[i] = 2;
            else if (res1 == f[i+2] + 1 && res2 == f[i+1] + 1) a[i-1] = 1, a[i] = 2;
            else if (res1 == f[i+1] + 1 && res2 == f[i+2] + 2) a[i-1] = 2, a[i] = 1;
            else a[i-1] = a[i] = 1;
            f[i-1] = f[i-1 + a[i]] + 1;
            f[i] = f[i-1 + a[i-1]] + 1; 
        }
        if (n & 1) {
            int res;
            cout << "throw 1" << endl;
            cin >> res;
            if (res == f[2] + 1) a[1] = 1;
            else a[1] = 2;
        }
        cout << "!";
        for (int i = 1; i <= n; i++)
            cout << ' ' << a[i];
        cout << endl;
    }
    return 0;
}