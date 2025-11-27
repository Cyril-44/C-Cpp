#include <bits/stdc++.h>
using namespace std;
constexpr int N = 200005;
int a[N << 1];
int main() {
    ios::sync_with_stdio(false);
    // cin.tie(nullptr);
    int T, n;
    cin >> T;
    while (T--) {
        cin >> n;
        memset(a, -1, sizeof(int) * (n<<1|1));
        a[n] = a[n<<1] = n;
        int l = n&1 ? 1 : 2, r = n&1 ? n+1 : n+1;
        for (int i = n-2; i > 1; i--) {
            if (i & 1) a[l] = a[l + i] = i, ++l;
            else a[r] = a[r + i] = i, ++r;
        }
        bool hasn_1 = false;
        for (int i = 1; i <= (n<<1); i++) {
            if (~a[i]) continue;
            if (!hasn_1 && i + (n-1) <= (n<<1) && a[i+(n-1)] == -1) a[i] = a[i+(n-1)] = n-1, hasn_1 = true;
            else a[i] = 1;
        }
        for (int i = 1; i <= (n<<1); i++)
            cout << a[i] << ' ';
        cout << '\n';
    }
    cout << flush;
    return 0;
}