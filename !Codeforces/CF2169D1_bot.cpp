#include <bits/stdc++.h>
using namespace std;
using int64 = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if (!(cin >> T)) return 0;
    const int64 MAXV = 1000000000000LL;
    while (T--) {
        long long x, y, k;
        cin >> x >> y >> k;

        auto survivors = [&](int64 v)->int64{
            int64 n = v;
            long long remSteps = x;
            while (remSteps > 0 && n >= y) {
                int64 q = n / y; // amount removed each operation now
                // number of consecutive steps while q stays the same
                // q*y <= n < (q+1)*y  => after s steps n - s*q >= q*y
                // max s satisfying: s <= (n - q*y) / q
                int64 maxSame = (n - q * y) / q + 1;
                if (maxSame <= 0) maxSame = 1;
                int64 take = (int64)min<long long>(remSteps, maxSame);
                n -= q * take;
                remSteps -= take;
            }
            return n;
        };

        if (survivors(MAXV) < k) {
            cout << -1 << '\n';
            continue;
        }

        int64 lo = 1, hi = MAXV;
        while (lo < hi) {
            int64 mid = lo + (hi - lo) / 2;
            if (survivors(mid) >= k) hi = mid;
            else lo = mid + 1;
        }
        cout << lo << '\n';
    }
    return 0;
}
