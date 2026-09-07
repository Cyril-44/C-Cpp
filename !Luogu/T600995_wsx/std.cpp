#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef long long ll;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    ll n, m, x;
    if (!(cin >> n >> m >> x)) return 0;
    vector<ll> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];

    sort(a.begin(), a.end());

    // 中位数位置在排序后的 (n-1)/2 (0-indexed)
    // 范围是 [a[mid] - m*x, a[mid] + m*x]
    if (m == 0 || x == 0) {
        cout << 1 << endl;
    } else {
        // 使用 __int128 避免 2 * m * x 溢出 long long
        unsigned __int128 ans = (unsigned __int128)2 * m * x + 1;
        string res = "";
        if (ans == 0) res = "0";
        while (ans > 0) {
            res += (char)((ans % 10) + '0');
            ans /= 10;
        }
        reverse(res.begin(), res.end());
        cout << res << endl;
    }

    return 0;
}