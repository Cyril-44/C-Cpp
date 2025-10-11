#include <bits/stdc++.h>
#include <limits>
using namespace std;
constexpr int N = 400005;
int a[N], b[N];
long long c[N];
inline long long calc(int l, int r) {
    if (l >= r) return 0;
    if (r - l == 2) {
        long long res = numeric_limits<long long>::max();
        for (int i = l; i <= r; i++) {
            long long sum = 0;
            for (int j = l; j <= r; j++) {
                sum += abs(a[j] - a[i]);
            }
            res = min(res, sum);
        }
        return res;
    }
    int len = r - l + 1;
    if (len & 1) {
        long long res = numeric_limits<long long>::max();
        for (int i = l; i+2 <= r; i += 2)
            res = min(res, calc(i, i+2) + calc(l, i-1) + calc(i+3, r));
        return res;
    }
    return c[r-1] - c[l-2];
}
int main() {
    cin.tie(nullptr) -> sync_with_stdio(false);
    int T, n;
    cin >> T;
    while (T--) {
        cin >> n;
        for (int i = 1; i <= n; i++) cin >> a[i], a[i+n] = a[i];
        for (int i = 1; i < 2*n; i++) b[i] = abs(a[i+1] - a[i]);
        c[1] = b[1];
        for (int i = 2; i < 2*n; i++) c[i] = c[i-2] + b[i];
        int is = 2;
        for (; is <= n; is++) if (a[is] == a[is+1]) break;
        long long ans = 0;
        for (int i = is; i <= is+n-1; ) {
            if (a[i] == a[i-1] || a[i] == a[i+1]) {++i; continue;}
            int j = i+1;
            for (; j <= is+n-1 && a[j] != a[j-1] && a[j] != a[j+1] && j-i <= n; j++);
            j -= 1;
            int len = j - i + 1;
            long long add = numeric_limits<long long>::max();
            if (len >= 2) add = min(add, calc(i, j));
            if (len-1 >= 2) {
                add = min(add, calc(i, j-1) + b[j]);
                add = min(add, calc(i+1, j) + b[i-1]);
            } else if (len == 1) {
                add = min(add, 0ll + b[i-1]);
                add = min(add, 0ll + b[j]);
            }
            if (len-2 >= 2) {
                add = min(add, calc(i+1, j-1) + b[i-1] + b[j]);
            } else if (len == 2) {
                add = min(add, 0ll + b[i-1] + b[j]);
            }
            ans += add;
            i = j+1;
        }
        cout << ans << '\n';
    }
    return 0;
}