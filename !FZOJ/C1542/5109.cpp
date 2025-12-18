#include <bits/stdc++.h>
using namespace std;
using Int = unsigned __int128;
ostream& operator<<(ostream& os, Int x) {
    int sta[45];
    sta[sta[0] = 1] = x % 10;
    while (x /= 10) sta[++sta[0]] = x % 10;
    while (sta[0]) os.put(sta[sta[0]--] | '0');
    return os;
}
int t[15];
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T; cin >> T;
    while (T--) {
        int m;
        unsigned t0;
        cin >> m >> t0;
        for (int i = 1; i <= m; ++i) cin >> t[i];

        Int ans = 1;
        for (int p = 0; p < 31; ++p) {
            if ((t0 >> p) & 1u) {
                int cp = 0;
                for (int i = 1; i <= m; ++i) {
                    if ((t[i] >> p) & 1u) ++cp;
                }
                ans *= (cp + 1);
            }
        }
        cout << ans << "\n";
    }
    return 0;
}
