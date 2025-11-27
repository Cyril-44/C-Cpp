#include <bits/stdc++.h>
using namespace std;
constexpr int N = 250005;
bool a[N];
int cnt0[N], cnt1[N];
int diffs[N];
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T, n, q;
    cin >> T;
    while (T--) {
        cin >> n >> q;
        for (int i = 1; i <= n; i++) cin >> a[i];
        for (int i = 1; i <= n; i++) {
            cnt1[i] = cnt1[i-1];
            cnt0[i] = cnt0[i-1];
            if (a[i]) ++cnt1[i];
            else ++cnt0[i];
        }
        for (int i = 2; i <= n; i++) {
            diffs[i] = diffs[i-1];
            if (a[i] ^ a[i-1]) ++diffs[i];
        }
        for (int l, r; q--; ) {
            cin >> l >> r;
            if ((cnt1[r] - cnt1[l-1]) % 3 || (cnt0[r] - cnt0[l-1]) % 3) {
                cout << "-1\n"; continue;
            }
            if (diffs[r] - diffs[l] == r - l) cout << (r - l + 1) / 3 + 1 << '\n';
            else cout << (r - l + 1) / 3 << '\n';
        }
    }
    return 0;
}
/*
12 q
0 1 1 0 1 0 1 0 0 1 0 1 
1 12 (expect: 4)
*/