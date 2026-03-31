#include <iostream>

using namespace std;

typedef long long ll;

// 计算 [1, x] 范围内的闰年总数
ll count_leap(ll x, ll a, ll b, ll c) {
    if (x <= 0) return 0;
    return x / a - x / b + x / c;
}

void solve() {
    ll m, a, b, c, n;
    if (!(cin >> m >> a >> b >> c >> n)) return;

    ll target = count_leap(m - 1, a, b, c) + n;
    ll low = m, high = 2e18; // 根据题目数据范围可调整上界
    ll ans = high;

    while (low <= high) {
        ll mid = low + (high - low) / 2;
        if (count_leap(mid, a, b, c) >= target) {
            ans = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    cout << ans << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int id, t;
    if (!(cin >> id >> t)) return 0;
    while (t--) {
        solve();
    }
    return 0;
}