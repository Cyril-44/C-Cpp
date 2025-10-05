#include <bits/stdc++.h>
using namespace std;
using int64 = long long;
using i128 = __int128_t;
 
i128 sumDigitsUpTo(int64 N) {
    if (N <= 0) return 0;
    i128 ans = 0;
    int64 w = 1;
    while (w <= N) {
        int64 higher = N / (w * 10);
        int64 cur = (N / w) % 10;
        int64 lower = N % w;
        // contribution at this position:
        // higher * 45 * w + (cur*(cur-1)/2) * w + cur * (lower + 1)
        ans += (i128)higher * 45 * (i128)w;
        ans += (i128)(cur * (cur - 1) / 2) * (i128)w;
        ans += (i128)cur * (i128)(lower + 1);
        if (w > (INT64_MAX / 10)) break; // avoid overflow when w*10 would overflow
        w *= 10;
    }
    return ans;
}
 
string toString(i128 x) {
    if (x == 0) return "0";
    bool neg = false;
    if (x < 0) { neg = true; x = -x; }
    string s;
    while (x > 0) {
        int digit = (int)(x % 10);
        s.push_back(char('0' + digit));
        x /= 10;
    }
    if (neg) s.push_back('-');
    reverse(s.begin(), s.end());
    return s;
}
 
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int T;
    if (!(cin >> T)) return 0;
    vector<int64> pow10(20,1);
    for (int i = 1; i < 20; ++i) pow10[i] = pow10[i-1] * 10LL;
 
    while (T--) {
        int64 k;
        cin >> k;
        i128 ans = 0;
        for (int d = 1; d <= 18 && k > 0; ++d) {
            int64 start = (d == 1) ? 1 : pow10[d-1];
            int64 cnt = 9LL * pow10[d-1];
            // total digits in this block:
            // Be careful with overflow: cnt * d fits in 128, but compare with k (<=1e15)
            i128 blockDigits = (i128)cnt * d;
            if ((i128)k >= blockDigits) {
                // take whole block: sum digits of numbers start..start+cnt-1
                int64 a = start;
                int64 b = start + cnt - 1;
                i128 sumBlock = sumDigitsUpTo(b) - sumDigitsUpTo(a - 1);
                ans += sumBlock;
                k -= (int64)blockDigits;
            } else {
                // partial in this block
                int64 full_nums = k / d; // number of whole numbers we can take
                int64 rem = k % d;       // leading digits from next number
                if (full_nums > 0) {
                    int64 a = start;
                    int64 b = start + full_nums - 1;
                    ans += sumDigitsUpTo(b) - sumDigitsUpTo(a - 1);
                }
                if (rem > 0) {
                    int64 num = start + full_nums;
                    string s = to_string(num);
                    // sum first rem digits (leftmost)
                    for (int i = 0; i < (int)rem; ++i) ans += (i128)(s[i] - '0');
                }
                k = 0;
                break;
            }
        }
        cout << toString(ans) << "\n";
    }
    return 0;
}
