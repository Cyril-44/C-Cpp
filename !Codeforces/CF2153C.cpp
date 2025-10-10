#include <bits/stdc++.h>
using namespace std;
constexpr int N = 200005;
int a[N], b[N];
int main() {
    cin.tie(nullptr) -> sync_with_stdio(false);
    int T, n;
    cin >> T;
    while (T--) {
        cin >> n;
        for (int i = 1; i <= n; i++) cin >> a[i];
        sort(a+1, a+1 + n);
        int cnt1 = 0;
        long long cnt2 = 0;
        long long sum = 0;
        for (int i = 1; i <= n; ) {
            int j = i+1;
            for (; j <= n && a[j] == a[i]; ++j);
            int count = j - i;
            if (count & 1) {
                b[++cnt1] = a[i];
            }
            cnt2 += count >> 1;
            sum += (count >> 1) * 2ll * a[i];
            i = j;
        }
        if (sum == 0) cout << "0\n";
        else {
            // Ask ext 2
            long long ans = sum;
            for (int i = 1; i <= cnt1; i++) {
                if (i < cnt1 && b[i+1] - b[i] < sum)
                    ans = max(ans, sum + b[i+1] + b[i]);
                if (b[i] < sum) ans = max(ans, sum + b[i]);
            }
            if (cnt2 == 1 && ans == sum) cout << "0\n"; // Two side
            else cout << ans << '\n';
        }
    }
    return 0;
}